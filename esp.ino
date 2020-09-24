#include <WiFi.h>

#include "api.h"

const char *wifi_ssid       = "test";
const char *wifi_password   = "password";

const char *api_address     = "nabucodonosor.xyz";
const int   api_port        = 32500;

const char *analyze_address = "https://insegreto.com/it/fresh";

void setup() {
  Serial.begin(115200);

  // WiFi setup
  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting to WiFi ");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  auto status = server_get_status(client, analyze_address, api_address, api_port);
  print_debug_status(status);

  delay(10000);
}
