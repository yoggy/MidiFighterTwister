#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include "MidiFighterTwister.h"

int main()
{
	BOOL rv;
	cc_msg msg;

	rv = mftStart();
	if (rv == FALSE) {
		_tprintf(_T("error: mftStart() failed...\n"));
		return -1;
	}

	mftSetKnobValue(0, 63);
	for (int i = 0; i < 180 * 60; ++i) {
		Sleep(15);
		while (true) {
			rv = mftPeekMessage(&msg);
			if (rv == FALSE) break;
			//mftDumpMessage(&msg);
		}

		for (int j = 1; j < 16; ++j) {
			mftSetKnobValue(j, mftGetKnobValue(0));
		}
	}

	mftStop();

    return 0;
}

