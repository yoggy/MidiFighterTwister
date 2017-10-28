#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include <stdint.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <vector>
#include <deque>

#define MIDIFIGHTERTWISTER_EXPORTS
#include "MidiFighterTwister.h"
#include "CriticalSection.h"
#include "KnobStatus.h"

static HMIDIIN handle_in_;
static HMIDIOUT handle_out_;

std::vector<KnobStatus> nob_status_;
std::deque<cc_msg> msg_queue_;
CriticalSection cs_;

extern "C" {
	void on_receive(uint8_t status, uint8_t channel, uint8_t data1, uint8_t data2);
	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	void send_cc_message(const uint8_t &status, const uint8_t &channel, const uint8_t &controller_number, const uint8_t &controller_data);

	int mftGetDevNum_()
	{
		int num_in = midiInGetNumDevs();
		int num_out = midiOutGetNumDevs();

		if (num_in != num_out) return 0;

		return num_in;
	}

	// for innner use only...
	void mftSetup_()
	{
		int num = mftGetDevNum_();
		_tprintf(_T("mftGetDevNum_() : num=%d\n"), num);

		nob_status_.resize(64);
		for (unsigned int i = 0; i < nob_status_.size(); ++i) {
			nob_status_[i].id = i;
		}
		msg_queue_.clear();
	}

	// for innner use only...
	void mftTeardown_()
	{
	}

	EXPORT_API BOOL mftStart()
	{
		if (handle_in_ != NULL) return FALSE;

		MMRESULT rv;
		int device_in_idx = 0;
		int device_out_idx = 1;

		rv = midiInOpen(&handle_in_, device_in_idx, (DWORD)(void*)midi_in_proc_static_, (DWORD_PTR)NULL, CALLBACK_FUNCTION);
		if (rv != MMSYSERR_NOERROR) {
			TCHAR buf[256];
			midiInGetErrorText(rv, buf, 256);
			_tprintf(_T("mftStart() : midiInOpen() failed...err=%s\n"), buf);

			return FALSE;
		}

		rv = midiOutOpen(&handle_out_, device_out_idx, NULL, NULL, CALLBACK_NULL);
		if (rv != MMSYSERR_NOERROR) {
			TCHAR buf[256];
			midiOutGetErrorText(rv, buf, 256);
			_tprintf(_T("mftStart() : midiOutOpen() failed...err=%s\n"), buf);

			midiInClose(handle_in_);
			handle_in_ = NULL;

			return FALSE;
		}

		midiInStart(handle_in_);

		return TRUE;
	}

	EXPORT_API void mftStop()
	{
		if (handle_in_ != NULL) {
			midiInStop(handle_in_);
			midiInClose(handle_in_);
			handle_in_ = NULL;
		}
		if (handle_out_ != NULL) {
			midiOutReset(handle_out_);
			midiOutClose(handle_out_);
			handle_out_ = NULL;
		}
	}

	EXPORT_API BOOL mftPeekMessage(cc_msg *msg)
	{
		if (msg == NULL) return FALSE;
		if (msg_queue_.size() == 0) return FALSE;

		{
			volatile ScopedCriticalSection scs(cs_);
			*msg = msg_queue_[0];
			msg_queue_.pop_front();
		}

		return TRUE;
	}

	EXPORT_API void mftDumpMessage(cc_msg *msg)
	{
		_tprintf(_T("msg={status:%02x, channel:%02x, controller_number:%02x, controller_data:%02x} \n"), 
			msg->status,
			msg->channel,
			msg->controller_number,
			msg->controller_data
		);
	}

	EXPORT_API uint8_t mftGetKnobValue(uint8_t idx)
	{
		volatile ScopedCriticalSection scs(cs_);
		if (nob_status_.size() <= idx) return 0;

		return nob_status_[idx].knob_value;
	}

	EXPORT_API void mftSetKnobValue(uint8_t idx, uint8_t value)
	{
		volatile ScopedCriticalSection scs(cs_);
		if (nob_status_.size() <= idx) return;

		nob_status_[idx].knob_value = value;
		send_cc_message(0xb0, 0, idx, value);
	}

	EXPORT_API BOOL mftIsKnobPressed(uint8_t idx)
	{
		volatile ScopedCriticalSection scs(cs_);
		if (nob_status_.size() <= idx) return 0;

		return nob_status_[idx].button_value > 0 ? TRUE : FALSE;
	}

	EXPORT_API void mftSetKnobColor(uint8_t idx, uint8_t value)
	{
		volatile ScopedCriticalSection scs(cs_);
		if (nob_status_.size() <= idx) return;

		send_cc_message(0xb0, 1, idx, value);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		uint8_t ch_msg, data1, data2, data3, status, channel;

		switch (wMsg) {
		case MIM_OPEN:
			break;
		case MIM_CLOSE:
			break;
		case MIM_DATA:
			ch_msg = (uint8_t)(0xFF & dwParam1);
			data1 = (uint8_t)(0xFF & (dwParam1 >> 8));
			data2 = (uint8_t)(0xFF & (dwParam1 >> 16));
			data3 = (uint8_t)(0xFF & (dwParam1 >> 24));

			status = ch_msg & 0xf0;
			channel = ch_msg & 0x0f;
			
			if (status == 0xb0) {
				// cc message only...
				on_receive(status, channel, data1, data2);
			}
			break;
		case MIM_LONGDATA:
			break;
		case MIM_ERROR:
			break;
		case MIM_LONGERROR:
			break;
		case MIM_MOREDATA:
			break;
		default:
			break;
		}
		return;
	}

	void on_receive(uint8_t status, uint8_t channel, uint8_t controller_number, uint8_t controller_data)
	{
		cc_msg msg;
		msg.status = status;
		msg.channel = channel;
		msg.controller_number = controller_number;
		msg.controller_data = controller_data;

		// for Midi Fighter Twister...
		int knob_idx = msg.controller_number;
		int value = msg.controller_data;

		{
			volatile ScopedCriticalSection scs(cs_);
			msg_queue_.push_back(msg);

			if (msg.channel == 0) {
				// knob value
				nob_status_[knob_idx].knob_value = value;				
			}
			else if (msg.channel == 1) {
				// press knob value
				nob_status_[knob_idx].button_value = value;
			}
		}
	}

	void send_cc_message(const uint8_t &status, const uint8_t &channel, const uint8_t &controller_number, const uint8_t &controller_data)
	{
		uint8_t ch_msg = 0;
		uint32_t data = 0;

		ch_msg = (status & 0xf0) | (channel & 0x0f);

		data |= (0x000000ff) & (ch_msg << 0);
		data |= (0x0000ff00) & (controller_number << 8);
		data |= (0x00ff0000) & (controller_data << 16);

		MMRESULT rv = midiOutShortMsg(handle_out_, data);
		if (rv != MMSYSERR_NOERROR) {
			TCHAR buf[256];
			midiOutGetErrorText(rv, buf, 256);
			_tprintf(_T("midi_message_send() : midiOutShortMsg() failed...err=%s\n"), buf);
		}
	}

}



