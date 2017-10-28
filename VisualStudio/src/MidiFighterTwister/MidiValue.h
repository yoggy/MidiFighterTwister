//
//	MidiValue.h
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

#include "CriticalSection.h"
#include <stdint.h>

class MidiValue {
public:
	MidiValue();
	~MidiValue();

	void value(const uint8_t &val, const bool &update=true);
	uint8_t value();
	bool is_updated();

private:
	// noncopyable
	MidiValue(const MidiValue &obj) {}
	MidiValue& operator=(const MidiValue &obj) { return *this; }

private:
	bool is_updated_;
	uint8_t value_;

	CriticalSection cs_;
};
