#include <Arduino.h>
#include "gpio_conf.h"
#include "vfd_display.h"
#include "wifi_adapter.h"

uint8_t ledStatus = 0;

TaskHandle_t mainTask;
TaskHandle_t handleDisplayTask;

BTC_rate rate;

void initTasks();

void setup() {
	Serial.begin(115200);

	wifi_connectToRouter();

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
	while (true) {
		wifi_fetchBtcRate(&rate);
		VFD_showBtcRate(&rate);

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