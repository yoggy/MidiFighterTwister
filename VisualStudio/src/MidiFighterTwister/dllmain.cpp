#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
	extern void mftSetup_();
	extern void mftTeardown_();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		mftSetup_();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		mftTeardown_();
		break;
	}
	return TRUE;
}

