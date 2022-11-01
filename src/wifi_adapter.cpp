#include "wifi_adapter.h"

static StaticJsonDocument<200> jsonDoc;
static HTTPClient http;

void wifi_connectToRouter() {
	WiFi.mode(WIFI_STA);
	WiFi.begin(ROUTER_SSID, ROUTER_PASS);
	Serial.print("Connecting to WiFi ..");

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(1000);
	}

	Serial.println("Successfully connected to router!");
	Serial.print("My IP address: ");
	Serial.println(WiFi.localIP());
}

void wifi_fetchBtcRate(BTC_rate *rate) {
	http.begin(LIVE_BTC_ENDPOINT);
	uint16_t httpResponseCode = http.GET();

	if (httpResponseCode == 200) {
		String payload = http.getString();
		DeserializationError error = deserializeJson(jsonDoc, payload);

		if (error) {
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.f_str());
		} else {
			rate->currentPrice = jsonDoc["currentPrice"];
			rate->previousPrice = jsonDoc["previousPrice"];
			rate->difference = jsonDoc["difference"];
		}
	} else {
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);
	}

    http.end();
}