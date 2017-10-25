#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include "MidiFighterTwister.h"

int main()
{
	int num = mftGetDevNum();
	_tprintf(_T("num=%d\n"), num);

	BOOL rv = mftStart(0);
	_tprintf(_T("rv=%d\n"), rv);


	for (int i = 0; i < 3 * 60; ++i) {
		Sleep(15);
		mftPeekMessage();
	}

	mftStop();

    return 0;
}

