#include <WiFi.h>

#include "api.h"

// Change this to connect to different wireless access point
const char *wifi_ssid          = "iPhone";
const char *wifi_password      = "12345678";

// Api address and port configuration
const char *api_address        = "nabucodonosor.xyz";
const int   api_port           = 32500;

// Web server endpoint
const char *analyze_address    = "https://insegreto.com/it/fresh";
const int   request_delay      = 5000; // Default time between each request
const int   duration_threshold = 5000; // Max accepted valid response time

// GPIO configuration
#define LED_RED     5
#define LED_GREEN   17
#define LED_BLUE    19
#define LED_LOADING 34

#define BUZZER 11

#define REQUEST_BUTTON 12

namespace led {
  void error() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_LOADING, LOW);
    digitalWrite(BUZZER, HIGH);
  }
  
  void ok() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_LOADING, LOW);
    digitalWrite(BUZZER, LOW);
  }
  
  void loading() {
    digitalWrite(LED_LOADING, HIGH);
  }
}

void setup() {
  // Initialize LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_LOADING, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_LOADING, HIGH);

  Serial.begin(9600);
  
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

  Serial.println("Sending request...");
  led::loading(); // Set led loading status
  
  // Initialize request
  auto s = server_get_status(client, analyze_address, api_address, api_port);

  // Check if error or duration time exceeds threshold
  if (s.error || s.duration_ms >= duration_threshold) {
    led::error();
  } else {
    led::ok();
  }
  
  print_debug_status(s);

  long int start_timer = millis();
  long int current_timer = millis();

  do {
    bool button_status = digitalRead(REQUEST_BUTTON);
    current_timer = millis();
  } while (button_status == LOW && (current_timer - start_timer) <= request_delay)
}

