#include "vfd_display.h"

uint32_t VFD_setBinary(uint32_t binary, uint8_t pos, uint8_t flagVal) {
	if (flagVal == 1) {
		return binary | (1 << pos);
	}

	return binary & (~(1 << pos));
}

void VFD_setOutput(uint32_t outputValue) {
	digitalWrite(VFD_LOAD_PIN, LOW);

	uint8_t bitValue;

	for (uint8_t i = 0; i < 32; i++) {
		bitValue = GET_BIT_VALUE(outputValue, i);

		digitalWrite(VFD_DIN_PIN, bitValue);
		digitalWrite(VFD_CLK_PIN, HIGH);
		delayMicroseconds(1);
		digitalWrite(VFD_CLK_PIN, LOW);
		delayMicroseconds(1);
	}

	digitalWrite(VFD_LOAD_PIN, HIGH);
	delayMicroseconds(1);
	digitalWrite(VFD_LOAD_PIN, LOW);
}

void VFD_showDigit(uint8_t digit, uint8_t value) {
	uint32_t outputValue = VFD_segmentMap[value];

	outputValue = VFD_setBinary(outputValue, VFD_digitMap[digit], 1);

	VFD_setOutput(outputValue);
}