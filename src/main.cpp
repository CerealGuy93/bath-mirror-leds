#include <Arduino.h>
#include "WiFi.h"
#include "LedCommand.h"
#include "IdleCommand.h"
#include "LedWrapper.h"
#include "BrushCommand.h"
#include "config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebserver.h>

LedWrapper wrapper;
LedCommand* currentCommand = new IdleCommand(wrapper);

AsyncWebServer* server = new AsyncWebServer(80);

void setup() {
  Serial.begin(115200);

  Serial.printf("Connecting to %s ", SSID);

  WiFi.begin(SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println();
  Serial.println("IP-Adress: " + WiFi.localIP().toString());


  server->on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    currentCommand = new IdleCommand(wrapper);
  });

  server->on("/brushing", HTTP_GET, [] (AsyncWebServerRequest *request) {
    currentCommand = new BrushCommand(wrapper);
  });

  server->begin();
}

void loop() {
  if (currentCommand)
    currentCommand->Execute();
}
