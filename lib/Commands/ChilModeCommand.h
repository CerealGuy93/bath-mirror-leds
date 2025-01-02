#ifndef ChilModeCommand_H
#define ChilModeCommand_H

#include "Command.h"

class ChilModeCommand : public Command {
    public:
        ChilModeCommand() : Command(NULL, true, 1000) { }

    protected:
        void OnInit() override {
            fill_gradient_RGB(leds, NUM_LEDS, CRGB::Green, CRGB::Orange);

            FastLED.show();
        }
};

#endif