#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <vector>
#include <Button.h>
#include <Command.h>
#include <IdleCommand.h>
#include <Renderer.h>
#include <TimerCommand.h>
#include <ChilModeCommand.h>

int currentBrightness = 3;

Command* currentCommand = NULL;

void setIdleCommand() {
  currentCommand = new IdleCommand();
  currentCommand->OnInit();
}

std::vector<MenuEntry> entries { 
  MenuEntry("Nur Licht", [] {
    currentCommand = new IdleCommand();
    currentCommand->OnInit();
  }),
  MenuEntry("Zähneputzen", [] {
    currentCommand = new TimerCommand(&setIdleCommand, 60 * 3, CRGB::Green, CRGB::Red);
    currentCommand->OnInit();
  }),
  MenuEntry("Chilly-Modus", [] {
    currentCommand = new ChilModeCommand();
    currentCommand->OnInit();
  }),
  MenuEntry("Timer 5min", [] {
    currentCommand = new TimerCommand(&setIdleCommand, 60 * 5, CRGB::Green, CRGB::Red);
    currentCommand->OnInit();
  }),
  MenuEntry("Timer 10min", [] {
    currentCommand = new TimerCommand(&setIdleCommand, 60 * 10, CRGB::Green, CRGB::Red);
    currentCommand->OnInit();
  }),
  MenuEntry("Timer 15min", [] {
    currentCommand = new TimerCommand(&setIdleCommand, 60 * 15, CRGB::Green, CRGB::Red);
    currentCommand->OnInit();
  })
};
Renderer renderer(new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE), entries);

void updateBrightness() {
  
  if (currentBrightness == 0) {
    FastLED.setBrightness(32);
    Serial.println("Brightness set to 0");
    currentBrightness++;
  } else if (currentBrightness == 1) {
    FastLED.setBrightness(64);
    Serial.println("Brightness set to 1");
    currentBrightness++;
  } else if (currentBrightness == 2) {
    FastLED.setBrightness(128);
    Serial.println("Brightness set to 2");
    currentBrightness++;
  } else if (currentBrightness == 3) {
    FastLED.setBrightness(196);
    Serial.println("Brightness set to 3");
    currentBrightness++;
  } else if (currentBrightness == 4) {
    FastLED.setBrightness(255);
    Serial.println("Brightness set to 4");
    currentBrightness = 0;
  }

  FastLED.show();
}

void onRedSinglePress(void *button_handle, void *usr_data) {
  renderer.select();
}

void onBlackSinglePress(void *button_handle, void *usr_data) {
  renderer.nextEntry();
}

void onBlackDoublePress(void *button_handle, void *usr_data) {
  updateBrightness();
}

void setup() {
  Serial.begin(9600);

  Button *redButton = new Button(GPIO_NUM_4, false);
  redButton->attachSingleClickEventCb(&onRedSinglePress, NULL);

  Button *blackButton = new Button(GPIO_NUM_15, false);
  blackButton->attachSingleClickEventCb(&onBlackSinglePress, NULL);
  blackButton->attachDoubleClickEventCb(&onBlackDoublePress, NULL);
  
  renderer.setup();

  FastLED.addLeds<WS2815, DATA_PIN, RGB>(leds, NUM_LEDS);
  updateBrightness();
}

void loop() {
  if (!currentCommand)
    setIdleCommand();
  
  currentCommand->Execute();
}