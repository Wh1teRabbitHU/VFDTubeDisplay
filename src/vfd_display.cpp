#include "vfd_display.h"

static SemaphoreHandle_t lock;
static int8_t displayDigits[VFD_DIGIT_COUNT] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
static int16_t displayDots = 0;

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

void VFD_showDigit(uint8_t digit, int8_t value, uint8_t hasDot) {
	if (value == -1) {
		VFD_setOutput(0); // To keep the same brightness, we need to have teh same amount of blank delays every time

		return;
	}

	uint32_t outputValue = VFD_segmentMap[value];

	outputValue = VFD_setBinary(outputValue, VFD_digitMap[digit], 1);

	if (hasDot) {
		outputValue = VFD_setBinary(outputValue, SEG_P, 1);
	}

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

void VFD_setDots(uint16_t dots) {
	displayDots = dots;
}

void VFD_setDot(uint8_t digit, int8_t hasDot) {
	if (hasDot == 1) {
		displayDots = displayDots | (1 << digit);
	} else {
		displayDots = displayDots & (~(1 << digit));
	}
}

void VFD_showBtcRate(BTC_rate *rate) {
	uint32_t priceNormal = rate->currentPrice * 100;
	int16_t diffReal = rate->difference * 100;
	uint16_t diffNormal = abs(diffReal);

	uint8_t price_fraction1 = priceNormal % 10;
	uint8_t price_fraction0 = (priceNormal / 10) % 10;
	uint8_t price_digit0 = (priceNormal / 100) % 10;
	uint8_t price_digit1 = (priceNormal / 1000) % 10;
	uint8_t price_digit2 = (priceNormal / 10000) % 10;
	uint8_t price_digit3 = (priceNormal / 100000) % 10;
	uint8_t price_digit4 = (priceNormal / 1000000) % 10;

	uint8_t diff_fraction1 = diffNormal % 10;
	uint8_t diff_fraction0 = (diffNormal / 10) % 10;
	uint8_t diff_digit0 = (diffNormal / 100) % 10;
	uint8_t diff_digit1 = (diffNormal / 1000) % 10;
	uint8_t diff_digit2 = (diffNormal / 10000) % 10;

	VFD_setDigit(0, price_fraction1);
	VFD_setDigit(1, price_fraction0);
	VFD_setDigit(2, price_digit0);
	VFD_setDigit(3, price_digit1);
	VFD_setDigit(4, price_digit2);
	VFD_setDigit(5, price_digit3);
	VFD_setDigit(6, price_digit4);

	uint8_t diffOffset;

	if (diff_digit1 == 0) { // 1 digit long
		diffOffset = 2;

		VFD_setDots(0b1000000000100);
		VFD_setDigit(8, -1);
		VFD_setDigit(9, -1);
	} else if (diff_digit2 == 0) { // 2 digits long
		diffOffset = 1;

		VFD_setDots(0b0100000000100);
		VFD_setDigit(8, -1);
		VFD_setDigit(11 + diffOffset, diff_digit1);
	} else { // 3 or more digits long
		diffOffset = 0;

		VFD_setDots(0b0010000000100);
		VFD_setDigit(11 + diffOffset, diff_digit1);
		VFD_setDigit(12 + diffOffset, diff_digit2);
	}

	VFD_setDigit(8 + diffOffset, diff_fraction1);
	VFD_setDigit(9 + diffOffset, diff_fraction0);
	VFD_setDigit(10 + diffOffset, diff_digit0);

	VFD_setDigit(13, diffReal > 0 ? -1 : VFD_MINUS_INDEX);
}

void VFD_refreshDisplayTask(void * pvParameters) {
	while (true) {
		for (uint8_t i = 0; i < VFD_DIGIT_COUNT; i++) {
			VFD_showDigit(i, VFD_getDigit(i), GET_BIT_VALUE(displayDots, i));
			delayMicroseconds(500);
		}
	}
}