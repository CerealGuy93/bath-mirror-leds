#include <Adafruit_NeoPixel.h>

#ifndef LED_WRAPPER_H
#define LED_WRAPPER_H

#define PIN_SHORT_STRIPE 23
#define PIN_LARGE_STRIPE 22
#define COUNT_SHORT_STRIPE 36
#define COUNT_LARGE_STRIPE 84
#define COUNT_LEDS (COUNT_SHORT_STRIPE + COUNT_LARGE_STRIPE)

class LedWrapper {
    public:
        LedWrapper() {
            
            
            this->pixels_large = new Adafruit_NeoPixel(COUNT_LARGE_STRIPE, PIN_LARGE_STRIPE, NEO_GRB + NEO_KHZ800);
            this->pixels_short = new Adafruit_NeoPixel(COUNT_SHORT_STRIPE, PIN_SHORT_STRIPE, NEO_GRB + NEO_KHZ800);

            this->pixels_large->begin();
            this->pixels_short->begin();
        }

        void setAllLeds(int r, int g, int b) {
            pixels_large->fill(pixels_large->Color(r, g, b), 0, COUNT_LARGE_STRIPE - 1);
            pixels_short->fill(pixels_short->Color(r, g, b), 0, COUNT_SHORT_STRIPE - 1);
            show();
        }

        void setBrightness(int brightness) {
            this->pixels_large->setBrightness(brightness);
            this->pixels_short->setBrightness(brightness);
        }

        void renderProgress(int percent) {
            if (percent == 0) {
                this->clear();
                return;
            }

            auto ledsOn = COUNT_LEDS / 180.0f * percent;
            if (ledsOn > COUNT_LEDS) {
                this->blink(3);
                return;
            }

            auto ledInt = (int) round(ledsOn) - 1;
            Serial.printf("Turning on %d (%f) LEDS\n", ledInt, ledsOn);
            this->render(ledInt, 0, 0, 255);
        }

        void blink(int count) {
            for (int i = 0; i < count; i++) {
                setAllLeds(0, 0, 255);
                delayMicroseconds(1000 * 500);
                setAllLeds(0, 255, 0);
                delayMicroseconds(1000 * 500);
                
            }
        }

    private:
        Adafruit_NeoPixel *pixels_large;
        Adafruit_NeoPixel *pixels_short;

        void clear() {
            this->pixels_large->clear();
            this->pixels_short->clear();
        }

        void show() {
            this->pixels_large->show();
            this->pixels_short->show();
        }

        void render(int led, int r, int g, int b) {
            if (led > COUNT_LEDS)
                return;

            if (led <= COUNT_LARGE_STRIPE) {
               this->pixels_large->setPixelColor(led, this->pixels_large->Color(r, g, b));
            } else {
                auto corrected = led - COUNT_LARGE_STRIPE;
                this->pixels_short->setPixelColor(corrected, this->pixels_short->Color(r, g, b));
            }

            this->show();
        }
};
#endif