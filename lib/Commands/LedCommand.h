#ifndef LedCommand_H
#define LedCommand_H

#include "LedWrapper.h"

class LedCommand {
    public:
        bool repeat = false;

        LedCommand(LedWrapper wrapper, bool repeat, int tickMs = 1000, int ticksTotal = 100) {
            this->wrapper = wrapper;
            this->repeat = repeat;
            this->currentTick = 0;
            this->tickMs = tickMs;
            this->ticksTotal = ticksTotal;
        }

        void Execute() {
            if (millis() - lastCalled > this->tickMs) {
                lastCalled = millis();

                if (this->currentTick > this->ticksTotal)
                    this->currentTick = 0;
                else
                    this->currentTick++;

                    
                Update(this->currentTick);
            }
        }

        void Reset() {
            this->currentTick = 0;
        }

    protected:
        LedWrapper wrapper;
        virtual void Update(int percent) {}

    private:
        unsigned int lastCalled;
        int currentTick;
        int tickMs;
        int ticksTotal;
};

#endif