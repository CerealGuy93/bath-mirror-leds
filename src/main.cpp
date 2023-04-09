#include <Arduino.h>
#include "LedCommand.h"
#include "IdleCommand.h"
#include "LedWrapper.h"
#include "BrushCommand.h"
#include "WifiHandler.h"
#include "config.h"

LedWrapper wrapper;

LedCommand* currentCommand = new IdleCommand(wrapper);
WifiHandler wifi(wrapper);

void setup() {
  Serial.begin(115200);

  wifi.Setup();
}

void loop() {
  auto newCommand = wifi.CheckForNewCommand();
  if (newCommand != nullptr)
    currentCommand = newCommand;

  currentCommand->Execute();
}
