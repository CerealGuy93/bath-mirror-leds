#ifndef IdleCommand_H
#define IdleCommand_H

#include "LedCommand.h"
#include "LedWrapper.h"

class IdleCommand : public LedCommand {
    public:
        IdleCommand(LedWrapper wrapper) : LedCommand(wrapper, true) { }

    protected:
        void Update(int percent) override {
            this->wrapper.setAllLeds(255, 0, 0);
        }
};

#endif