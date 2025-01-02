#ifndef TimerCommand_H
#define TimerCommand_H

#include "Command.h"
#include <ColorSet.h>
#include <StaticStuff.h>

class TimerCommand : public Command {
    public:
        CRGB colorProgress, colorBackground;
        TimerCommand(void(*onFinish)(), int seconds, CRGB colorProgress, CRGB colorBackground) : Command(onFinish, false, 1000, seconds) {
            this->colorProgress = colorProgress;
            this->colorBackground = colorBackground;
        }

    protected:
        void Update(int percent) override {
            if (percent == 0) {
                FastLED.clear();
                FastLED.show();
                return;
            }

            auto ledsOn = NUM_LEDS / static_cast<float>(this->ticksTotal) * percent;
            if (ledsOn > NUM_LEDS) {
                this->blink(3);
                return;
            }

            auto ledInt = (int) floor(ledsOn);
            Serial.printf("Turning on %d (%f) LEDS\n", ledInt, ledsOn);

            FastLED.clear();

            for (int i = 0; i < ledInt; i++)
            {
                leds[i] = this->colorProgress;
            }


            for (int i = ledInt; i < NUM_LEDS; i++)
            {
                leds[i] = this->colorBackground;                
            }
            
            FastLED.show();
        }

        void blink(int count) {
            for (int i = 0; i < count; i++) {
                for (int i = 0; i < NUM_LEDS; i++)
                {
                    leds[i] = CRGB::Green;
                }
                FastLED.show();
                delayMicroseconds(1000 * 500);
                
                for (int i = 0; i < NUM_LEDS; i++)
                {
                    leds[i] = CRGB::YellowGreen;
                }
                FastLED.show();
                delayMicroseconds(1000 * 500);
            }
        }
};

#endif