#ifdef MIDIFIGHTERTWISTER_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

#include <stdint.h>

#ifndef MIDIFIGHTERTWISTER_EXPORTS
#if _DEBUG
#pragma comment(lib, "MidiFighterTwister32D.lib")
#else
#pragma comment(lib, "MidiFighterTwister32.lib")
#endif

#endif

extern "C" {
	struct cc_msg_ {
		uint8_t status;
		uint8_t channel;
		uint8_t controller_number;
		uint8_t controller_data;
	};
	typedef struct cc_msg_ cc_msg;

	// for public api
	EXPORT_API BOOL mftStart();
	EXPORT_API void mftStop();
	EXPORT_API BOOL mftPeekMessage(cc_msg *msg);
	EXPORT_API void mftDumpMessage(cc_msg *msg);

	EXPORT_API uint8_t mftGetKnobValue(uint8_t idx);
	EXPORT_API void mftSetKnobValue(uint8_t idx, uint8_t value);
	EXPORT_API BOOL mftIsKnobPressed(uint8_t idx);
	EXPORT_API void mftSetKnobColor(uint8_t idx, uint8_t value);
}
