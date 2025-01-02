#ifndef IdleCommand_H
#define IdleCommand_H

#include "Command.h"
#include <StaticStuff.h>

class IdleCommand : public Command {
    public:
        IdleCommand() : Command(NULL, true) { }

    protected:
        void OnInit() override {
            for(int led = 0; led < NUM_LEDS; led++) { 
                leds[led] = CRGB::Orange; 
            }

            FastLED.show();
        }
};

#endif