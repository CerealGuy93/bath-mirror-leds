#ifndef LedCommand_H
#define LedCommand_H

#include <StaticStuff.h>

class Command {
    public:
        bool repeat = false;

        Command(void(*onFinish)(), bool repeat, int tickMs = 1000, int ticksTotal = 100) {
            this->repeat = repeat;
            this->currentTick = 0;
            this->tickMs = tickMs;
            this->ticksTotal = ticksTotal;

            FastLED.clear();
            FastLED.show();

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

        virtual void OnInit() {}
    protected:
        virtual void Update(int percent) {}
        int ticksTotal;

    private:
        unsigned int lastCalled;
        int currentTick;
        int tickMs;
};

#endif