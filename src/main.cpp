#include <Arduino.h>
#include "gpio_conf.h"
#include "vfd_display.h"

uint8_t ledStatus = 0;
uint8_t numberCounter = 0;
uint8_t digitCounter = 0;

TaskHandle_t mainTask;
TaskHandle_t handleDisplayTask;

void initTasks();

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

	VFD_init();
	initTasks();
}

void mainTaskImpl(void * pvParameters) {
	Serial.print("Task1 running on core ");
	Serial.println(xPortGetCoreID());

	while (true) {
		for (uint8_t i = 0; i < VFD_DIGIT_COUNT; i++) {
			VFD_setDigit(i, i == digitCounter ? numberCounter : -1);
		}

		if (++numberCounter > 9) numberCounter = 0;
		if (++digitCounter > 12) digitCounter = 0;

		delay(1000);
	}
}

void loop() {
	delay(100);
}

void initTasks() {
	xTaskCreatePinnedToCore(mainTaskImpl, "mainTask", 10000, NULL, 1, &mainTask, 0);
	delay(500);
	xTaskCreatePinnedToCore(VFD_refreshDisplayTask, "handleDisplayTask", 10000, NULL, 2, &handleDisplayTask, 1);
    delay(500);
}