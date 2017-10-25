#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include <stdint.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MIDIFIGHTERTWISTER_EXPORTS
#include "MidiFighterTwister.h"

static HMIDIIN handle_in_;
static HMIDIOUT handle_out_;

extern "C" {
	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

	EXPORT_API int mftGetDevNum()
	{
		int num_in = midiInGetNumDevs();
		int num_out = midiOutGetNumDevs();

		if (num_in != num_out) return 0;

		return num_in;
	}

	EXPORT_API BOOL mftStart(const int &device_idx)
	{
		if (handle_in_ != NULL) return FALSE;

		MMRESULT rv;

		rv = midiInOpen(&handle_in_, device_idx, (DWORD)(void*)midi_in_proc_static_, (DWORD_PTR)NULL, CALLBACK_FUNCTION);
		if (rv != MMSYSERR_NOERROR) {
			TCHAR buf[256];
			midiInGetErrorText(rv, buf, 256);
			_tprintf(_T("mftStart() : midiInOpen() failed...err=%s\n"), buf);

			return FALSE;
		}

		rv = midiOutOpen(&handle_out_, device_idx, NULL, NULL, CALLBACK_NULL);
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

	EXPORT_API void mftPeekMessage()
	{

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CALLBACK midi_in_proc_static_(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		unsigned char ch_msg, data1, data2, data3;

		switch (wMsg) {
		case MIM_OPEN:
			_tprintf(_T("wMsg=MIM_OPEN\n"));
			break;
		case MIM_CLOSE:
			_tprintf(_T("wMsg=MIM_CLOSE\n"));
			break;
		case MIM_DATA:
			ch_msg = (unsigned char)(0xFF & dwParam1);
			data1 = (unsigned char)(0xFF & (dwParam1 >> 8));
			data2 = (unsigned char)(0xFF & (dwParam1 >> 16));
			data3 = (unsigned char)(0xFF & (dwParam1 >> 24));
			_tprintf(_T("wMsg=MIM_DATA, ch_msg=%02x, data1=%02x, data2=%02x, data3=%02x\n"), ch_msg, data1, data2, data3);
			break;
		case MIM_LONGDATA:
			_tprintf(_T("wMsg=MIM_LONGDATA\n"));
			break;
		case MIM_ERROR:
			_tprintf(_T("wMsg=MIM_ERROR\n"));
			break;
		case MIM_LONGERROR:
			_tprintf(_T("wMsg=MIM_LONGERROR\n"));
			break;
		case MIM_MOREDATA:
			_tprintf(_T("wMsg=MIM_MOREDATA\n"));
			break;
		default:
			_tprintf(_T("wMsg = unknown\n"));
			break;
		}
		return;
	}

	void midi_out_send(const int8_t &a, const int8_t &b, const int8_t &c, const int8_t &d)
	{
		uint32_t data = 0;

		data |= (0x000000ff) & (a << 0);
		data |= (0x0000ff00) & (b << 8);
		data |= (0x00ff0000) & (c << 16);
		data |= (0xff000000) & (d << 24);

		midiOutShortMsg(handle_out_, data);
	}

}



