//
//	MidiFighterTwister.h
//	
//	github:
//		https://github.com/yoggy/MidiFighterTwister
//
//  license:
//      Copyright(c) 2017 yoggy <yoggy0@gmail.com>
//      Released under the MIT license
//      http://opensource.org/licenses/mit-license.php;	
//
#pragma once

#ifdef MIDIFIGHTERTWISTER_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

#include <stdint.h>

#ifndef MIDIFIGHTERTWISTER_EXPORTS
	#if _WIN64
		#if _DEBUG
			#pragma comment(lib, "MidiFighterTwister64D.lib")
		#else
			#pragma comment(lib, "MidiFighterTwister64.lib")
		#endif
	#else
		#if _DEBUG
			#pragma comment(lib, "MidiFighterTwister32D.lib")
		#else
			#pragma comment(lib, "MidiFighterTwister32.lib")
		#endif
	#endif
#endif

extern "C" {
	EXPORT_API uint8_t mftStart();
	EXPORT_API void mftStop();

	EXPORT_API uint8_t mftIsUpdatedKnobValue(uint8_t idx);
	EXPORT_API uint8_t mftGetKnobValue(uint8_t idx);
	EXPORT_API void mftSetKnobValue(uint8_t idx, uint8_t value);

	EXPORT_API uint8_t mftIsUpdatedButtonValue(uint8_t idx);
	EXPORT_API uint8_t mftGetButtonValue(uint8_t idx);

	EXPORT_API void mftSetKnobColor(uint8_t idx, uint8_t value);

	// bank 0Å`3
	EXPORT_API uint8_t mftIsUpdatedBank();
	EXPORT_API uint8_t mftGetBank();
	EXPORT_API void mftSetBank(uint8_t bank_idx);

	EXPORT_API void mftSendCCMessage(uint8_t channel, uint8_t control_number, uint8_t control_data);
}
