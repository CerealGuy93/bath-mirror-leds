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
                currentTime = millis();
                previousTime = currentTime;
                String currentLine = "";

                while (client.connected() && currentTime - previousTime <= timeoutTime) {
                currentTime = millis();
                if (client.available()) { 
                    char c = client.read();
                    header += c;
                    
                    if (c == '\n') {

                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        
                        if (header.indexOf("GET /on") >= 0) {
                            command = new IdleCommand(this->wrapper);
                            Serial.println("Set IdleCommand");
                        } else if (header.indexOf("GET /brushing") >= 0) {
                            command = new BrushCommand(this->wrapper);
                            Serial.println("Set BrushingCommand");
                        }
                        
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        
                        client.println("<body><h1>ESP32 Web Server</h1>");
                        
                        client.println("</body></html>");
                        
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                    } else if (c != '\r') {
                        currentLine += c;
                    }
                }
                }

                header = "";
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

        String header;
};