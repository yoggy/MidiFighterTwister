#ifdef MIDIFIGHTERTWISTER_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

#ifndef MIDIFIGHTERTWISTER_EXPORTS
#if _DEBUG
#pragma comment(lib, "MidiFighterTwister32D.lib")
#else
#pragma comment(lib, "MidiFighterTwister32.lib")
#endif

#endif

extern "C" {
	EXPORT_API int 	mftGetDevNum();
	EXPORT_API BOOL mftStart(const int & device_idx);
	EXPORT_API void mftStop();
	EXPORT_API void mftPeekMessage();
}
