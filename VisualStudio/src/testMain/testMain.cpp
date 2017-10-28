//
//	testMain.cpp
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

#include "MidiFighterTwister.h"

int main()
{
	uint8_t rv;

	rv = mftStart();
	if (rv == FALSE) {
		_tprintf(_T("error: mftStart() failed...\n"));
		return -1;
	}

	mftSetBank(0);
	mftSetKnobValue(0, 63);
	for (int i = 0; i < 180 * 60; ++i) {
		Sleep(15);

		for (int idx = 0; idx < 64; ++idx) {
			if (mftIsUpdatedKnobValue(idx)) {
				_tprintf(_T("knob idx=%d, val=%d\n"), idx, mftGetKnobValue(idx));
			}
			if (mftIsUpdatedButtonValue(idx)) {
				_tprintf(_T("button idx=%d, val=%d\n"), idx, mftGetButtonValue(idx));
			}
		}
		if (mftIsUpdatedBank()) {
			_tprintf(_T("bank chenged bank_idx=%d\n"), mftGetBank());
		}
	}

	mftStop();

    return 0;
}

