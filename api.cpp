#include "api.h"

response_status server_get_status(WiFiClient client, const char *url, const char *api_address, const int api_port) {
  response_status s;

  if (!client.connect(api_address, api_port)) {
    Serial.println("Connection with API failed");

    s.error = true;
    s.status_code = API_CONNECTION_FAILED;
    s.duration_ms = -1;

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

  String response = client.readString();
  const char split = ',';

  String response_parsed[RESPONSE_LENGTH];

  size_t block_index = 0;
  for (size_t i = 0; i < response.length(); i++) {
    char current = response.charAt(i);

    if (block_index >= RESPONSE_LENGTH) {
      break;
    }

    if (current != split) {
      response_parsed[block_index].concat(current);
    } else {
      block_index++;
    }
  }

  s.error = response_parsed[0].toInt();
  s.status_code = (status_codes) response_parsed[1].toInt();
  s.duration_ms = response_parsed[2].toInt();

  return s;
}

void print_debug_status(response_status s) {
  if (s.error) {
  Serial.print("error: true, ");
  } else {
    Serial.print("error: false, ");
  }

  Serial.print("status_code: ");
  Serial.print(s.status_code);
  Serial.print(", ");

  Serial.print("duration_ms: ");
  Serial.println(s.duration_ms);
}
