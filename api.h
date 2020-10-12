#ifndef API_H
#define API_H

#include <WiFi.h>

#define RESPONSE_LENGTH 3 // response_status field count

// Throwable status codes
enum status_codes {
  API_CONNECTION_FAILED = -1,
  HTTP_OK = 200,
  HTTP_INTERNAL_SERVER_ERROR = 500,
  HTTP_BAD_GATEWAY = 502,
  HTTP_WEB_SERVER_DOWN = 521,
  HTTP_CONNECTION_TIMED_OUT = 522
};

struct response_status {
  bool error;
  enum status_codes status_code;
  unsigned int duration_ms;
};

/**
 * Send an API request and return web service status
*/
response_status server_get_status(WiFiClient client, const char *url, const char *api_address, const int api_port);

/**
 * Print useful debug info to Serial console
*/
void print_debug_status(response_status s);

#endif
