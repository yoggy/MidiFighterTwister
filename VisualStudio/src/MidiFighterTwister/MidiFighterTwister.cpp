//
//	MidiFighterTwister.cpp
//	
//	github:
//		https://github.com/yoggy/MidiFighterTwister
//
//  license:
//      Copyright(c) 2017 yoggy <yoggy0@gmail.com>
//      Released under the MIT license
//      http://opensource.org/licenses/mit-license.php;	
//
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
#include "MidiValue.h"

static HMIDIIN handle_in_;
static HMIDIOUT handle_out_;

#define KNOB_NUM 64

MidiValue knob_status_[KNOB_NUM];
MidiValue button_status_[KNOB_NUM];
MidiValue bank_;

extern "C" {
	struct midi_msg_ {
		uint8_t status;
		uint8_t channel;
		uint8_t data1;
		uint8_t data2;
	};
	typedef struct midi_msg_ midi_msg;

	struct cc_msg_ {
		uint8_t status;
		uint8_t channel;
		uint8_t control_number;
		uint8_t control_data;
	};
	typedef struct cc_msg_ cc_msg;

	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

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
	}

	// for innner use only...
	void mftTeardown_()
	{
	}

	EXPORT_API uint8_t mftStart()
	{
		if (handle_in_ != NULL) return FALSE;

		MMRESULT rv;
		int device_in_idx = 0;
		int device_out_idx = 1;

		rv = midiInOpen(&handle_in_, device_in_idx, (DWORD_PTR)midi_in_proc_static_, (DWORD_PTR)NULL, CALLBACK_FUNCTION);
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

	EXPORT_API uint8_t mftIsUpdatedKnobValue(uint8_t idx)
	{
		if (KNOB_NUM <= idx) return FALSE;

		return knob_status_[idx].is_updated() ? TRUE : FALSE;
	}

	EXPORT_API uint8_t mftGetKnobValue(uint8_t idx)
	{
		if (KNOB_NUM <= idx) return 0;

		return knob_status_[idx].value();
	}

	EXPORT_API void mftSetKnobValue(uint8_t idx, uint8_t value)
	{
		if (KNOB_NUM <= idx) return;

		knob_status_[idx].value(value);
		
		// send knob value to Twister
		mftSendCCMessage(0, idx, value);
	}

	EXPORT_API uint8_t mftIsUpdatedButtonValue(uint8_t idx)
	{
		if (KNOB_NUM <= idx) return FALSE;

		return button_status_[idx].is_updated() ? TRUE : FALSE;
	}

	EXPORT_API uint8_t mftGetButtonValue(uint8_t idx)
	{
		if (KNOB_NUM <= idx) return 0;

		return button_status_[idx].value();
	}

	EXPORT_API void mftSetKnobColor(uint8_t idx, uint8_t value)
	{
		if (KNOB_NUM <= idx) return;

		// send knob led color to Twister
		mftSendCCMessage(1, idx, value);
	}

	EXPORT_API uint8_t mftIsUpdatedBank()
	{
		return bank_.is_updated();
	}

	EXPORT_API uint8_t mftGetBank()
	{
		return bank_.value();
	}

	EXPORT_API void mftSetBank(uint8_t bank_idx)
	{
		if (3 < bank_idx) return;

		bank_.value(bank_idx);
		mftSendCCMessage(3, bank_idx, 0x7f);
	}

	void mftSendCCMessage(uint8_t channel, uint8_t control_number, uint8_t control_data)
	{
		uint8_t status = 0xb0;
		uint8_t ch_msg = 0;
		uint32_t data = 0;

		ch_msg = (status & 0xf0) | (channel & 0x0f);

		data |= (0x000000ff) & (ch_msg << 0);
		data |= (0x0000ff00) & (control_number << 8);
		data |= (0x00ff0000) & (control_data << 16);

		MMRESULT rv = midiOutShortMsg(handle_out_, data);
		if (rv != MMSYSERR_NOERROR) {
			TCHAR buf[256];
			midiOutGetErrorText(rv, buf, 256);
			_tprintf(_T("mftSendCCMessage() : midiOutShortMsg() failed...err=%s\n"), buf);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	midi_msg parse_midi_messsage_(uint32_t data)
	{
		midi_msg msg;

		uint8_t ch_msg = (uint8_t)(0xFF & data);
		msg.status = ch_msg & 0xf0;
		msg.channel = ch_msg & 0x0f;

		msg.data1 = (uint8_t)(0xFF & (data >> 8));
		msg.data2 = (uint8_t)(0xFF & (data >> 16));

		return msg;
	}

	void dump_midi_msg_(const midi_msg &msg)
	{
		_tprintf(_T("midi_msg={status:%02x, channel:%02x, data1:%02x, data2:%02x} \n"),
			msg.status,
			msg.channel,
			msg.data1,
			msg.data2
		);
	}

	cc_msg convert_to_cc_message_(const midi_msg &msg)
	{
		cc_msg cc;

		cc.status = msg.status;
		cc.channel = msg.channel;
		cc.control_number = msg.data1;
		cc.control_data = msg.data2;

		return cc;
	}

	void dump_cc_msg_(const cc_msg &cc)
	{
		_tprintf(_T("cc_msg={status:%02x, channel:%02x, control_number:%02x, control_data:%02x} \n"),
			cc.status,
			cc.channel,
			cc.control_number,
			cc.control_data
		);
	}

	void on_receive_cc_(const cc_msg &cc)
	{
		//dump_cc_msg_(cc);

		// for Midi Fighter Twister...
		int knob_idx = cc.control_number;
		int val = cc.control_data;

		if (cc.channel == 0) {
			// knob value
			knob_status_[knob_idx].value(val);
		}
		else if (cc.channel == 1) {
			// press/release value
			button_status_[knob_idx].value(val);
		}
		else if (cc.channel == 3) {
			// chenged bank
			if (0 <= cc.control_number && cc.control_number <= 3) {
				if (cc.control_data == 0x7f) {
					bank_.value(cc.control_number);
				}
			}
		}
	}

	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		midi_msg msg;

		switch (wMsg) {
		case MIM_OPEN:
			break;
		case MIM_CLOSE:
			break;
		case MIM_DATA:
			msg = parse_midi_messsage_(dwParam1);
			//dump_midi_msg_(msg);

			if (msg.status == 0xb0) {	
				on_receive_cc_(convert_to_cc_message_(msg));
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
}



