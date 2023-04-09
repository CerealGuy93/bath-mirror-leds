#include <WiFi.h>
#include "config.h"
#include "LedCommand.h"
#include "IdleCommand.h"
#include "LedWrapper.h"
#include "BrushCommand.h"

class WifiHandler {
    public:
        WifiHandler(LedWrapper wrapper) {
            this->wrapper = wrapper;
            this->server = WiFiServer(80);
        }

        void Setup() {
            Serial.printf("Connecting to %s ", SSID);
            WiFi.begin(SSID, WLAN_PASS);

            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }

            Serial.println();
            Serial.println("IP-Adress: " + WiFi.localIP().toString());
            this->server.begin();

        }

        LedCommand* CheckForNewCommand() {
            WiFiClient client = this->server.available();
            LedCommand* command = nullptr; 

            if (client) {
                auto message = ParseMessage(client);

                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();

                if (message.indexOf("GET /on") >= 0) {
                    command = new IdleCommand(this->wrapper);

                } else if (message.indexOf("GET /off") >= 0) {
                    command = new IdleCommand(this->wrapper);

                } else if (message.indexOf("GET /brushing") >= 0) {
                    command = new BrushCommand(this->wrapper);

                } else if (message.indexOf("POST /settings")) {
                    // TODO get settings from body
                    //{ "brightness": 0 - 100,  "ambientColor": "#00FF00"}
                    
                }

                
                /*
                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                
                client.println("<body><h1>ESP32 Web Server</h1>");
                
                client.println("</body></html>");
                */

                Serial.println("request ---");
                Serial.println(message);
                Serial.println("request END ---");

                client.stop();
            }
        
            return command;
        }

    private:
        WiFiServer server;
        LedWrapper wrapper;

        unsigned long currentTime = millis();
        unsigned long previousTime = 0; 
        const long timeoutTime = 2000;

        String ParseMessage(WiFiClient client) {
            currentTime = millis();
            previousTime = currentTime;
            String message = "";

            while (client.connected() && currentTime - previousTime <= timeoutTime) {
                currentTime = millis();
                if (client.available()) { 
                    char c = client.read();
                    message += c;
                }
            }

            return message;
        }
};