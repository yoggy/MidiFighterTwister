//
//	MidiValue.cpp
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

#include "MidiValue.h"

MidiValue::MidiValue() : is_updated_(false), value_(0)
{
	
}

MidiValue::~MidiValue()
{

}

void MidiValue::value(const uint8_t &val, const bool &update)
{
	volatile ScopedCriticalSection scs(cs_);
	this->is_updated_ = update;
	this->value_ = val;
}

uint8_t MidiValue::value()
{
	volatile ScopedCriticalSection scs(cs_);
	this->is_updated_ = false;

	return this->value_;
}

bool MidiValue::is_updated()
{
	volatile ScopedCriticalSection scs(cs_);
	return this->is_updated_;
}
