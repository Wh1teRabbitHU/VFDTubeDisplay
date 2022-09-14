#include <Arduino.h>
#include "gpio_conf.h"
#include "vfd_display.h"

uint8_t ledStatus = 0;
uint8_t numberCounter = 0;
uint8_t digitCounter = 0;

void setup() {
	Serial.begin(115200);

	pinMode(LED_01_PIN, OUTPUT);
	pinMode(LED_02_PIN, OUTPUT);

	pinMode(PREV_BTN_PIN, INPUT);

	pinMode(VFD_CLK_PIN, OUTPUT);
	pinMode(VFD_DIN_PIN, OUTPUT);
	pinMode(VFD_LOAD_PIN, OUTPUT);
	pinMode(VFD_BLANK_PIN, OUTPUT);

	digitalWrite(VFD_LOAD_PIN, LOW);
	digitalWrite(VFD_DIN_PIN, LOW);
	digitalWrite(VFD_CLK_PIN, LOW);
	digitalWrite(VFD_BLANK_PIN, LOW);
}

uint32_t setBinary(uint32_t binary, uint8_t pos, uint8_t flagVal) {
	if (flagVal == 1) {
		return binary | (1 << pos);
	}

	return binary & (~(1 << pos));
}

void loop() {
	for (uint8_t i = 0; i < 13; i++) {
		VFD_showDigit(i, i%10);
		delayMicroseconds(500);
	}
}