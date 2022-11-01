#ifndef WIFI_ADAPTER_H
#define WIFI_ADAPTER_H

#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "stdint.h"
#include "wifi_connection.h"

typedef struct BTC_rate {
	float currentPrice;
	float previousPrice;
	float difference;
} BTC_rate;

void wifi_connectToRouter();
void wifi_fetchBtcRate(BTC_rate *rate);

#endif