#include "api.h"

response_status server_get_status(WiFiClient client, const char *url, const char *api_address, const int api_port) {
  response_status s;

  if (!client.connect(api_address, api_port)) {
    Serial.println("Connection with API failed");

    s.error = true;
    return s;
  }

  // Send TCP packet with address payload
  client.print(url);

  // Wait response
  size_t tries = 0;
  while (!client.available() && tries < 60000) {
    tries++;
    delay(10);
  }

  while (client.available()) {
    String line = client.readString();
    Serial.println(line);
  }

  return s;
}
