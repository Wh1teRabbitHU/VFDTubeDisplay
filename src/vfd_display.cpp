#include "vfd_display.h"

static SemaphoreHandle_t lock;
static int8_t displayDigits[VFD_DIGIT_COUNT] = { -1 };

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

void VFD_showDigit(uint8_t digit, int8_t value) {
	if (value == -1) {
		VFD_setOutput(0); // To keep the same brightness, we need to have teh same amount of blank delays every time

		return;
	}

	uint32_t outputValue = VFD_segmentMap[value];

	outputValue = VFD_setBinary(outputValue, VFD_digitMap[digit], 1);

	VFD_setOutput(outputValue);
}

void VFD_init() {
	lock = xSemaphoreCreateMutex();
}

void VFD_setDigit(uint8_t digit, int8_t value) {
	xSemaphoreTake(lock, portMAX_DELAY);
	displayDigits[digit] = value;
	xSemaphoreGive(lock);
}

int8_t VFD_getDigit(uint8_t digit) {
	xSemaphoreTake(lock, portMAX_DELAY);
	int8_t value = displayDigits[digit];
	xSemaphoreGive(lock);

	return value;
}

void VFD_refreshDisplayTask(void * pvParameters) {
	while (true) {
		for (uint8_t i = 0; i < VFD_DIGIT_COUNT; i++) {
			VFD_showDigit(i, VFD_getDigit(i));
			delayMicroseconds(500);
		}
	}
}