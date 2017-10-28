#pragma once

#include <stdint.h>

class KnobStatus {
public:
	uint8_t id;

	bool update_knob_value;
	uint8_t knob_value;

	bool update_button_value;
	uint8_t button_value;
};
