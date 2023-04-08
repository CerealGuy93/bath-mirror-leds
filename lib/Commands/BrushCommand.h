#ifndef BrushCommand_H
#define BrushCommand_H

#include "LedCommand.h"
#include "LedWrapper.h"

class BrushCommand : public LedCommand {
    public:
        BrushCommand(LedWrapper wrapper) : LedCommand(wrapper, false, 1000, 180) { }

    protected:
        void Update(int percent) override {
            this->wrapper.renderProgress(percent);
        }
};

#endif