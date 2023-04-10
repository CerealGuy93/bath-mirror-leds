#include <Arduino.h>
#include "WiFi.h"
#include "LedCommand.h"
#include "IdleCommand.h"
#include "LedWrapper.h"
#include "BrushCommand.h"
#include "config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebserver.h>
#include <ArduinoJson.h>

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
    request->send(200);
  });

  server->on("/brushing", HTTP_GET, [] (AsyncWebServerRequest *request) {
    currentCommand = new BrushCommand(wrapper);
    request->send(200);
  });

  server->on("/settings", HTTP_POST, [] (AsyncWebServerRequest *request) {
    request->send(200);
  },
  NULL,
  [] (AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
    
    DynamicJsonDocument bodyJSON(1024);
    deserializeJson(bodyJSON, data, len);
    
    String brightness = bodyJSON["brightness"];
    if (brightness != "NULL" && brightness != "") {
      Serial.println("setBrightness to " + brightness);
      auto parsed = brightness.toInt();
      wrapper.setBrightness(parsed);      
    }
  });

  server->begin();
}

void loop() {
  if (currentCommand)
    currentCommand->Execute();
}
