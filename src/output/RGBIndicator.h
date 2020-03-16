#pragma once

#ifndef RGBIndicator_h
#define RGBIndicator_h
#include <Arduino.h>
#include "Color.h"
#include "RGBStatus.h"


class RGBIndicator{
  private:
    uint8_t pinRed, pinGreen, pinBlue;
    unsigned int interval = 500;
    int currentCount = 0, currentIndex = 0;

  public:
    bool reset = true;

  //methods
    RGBIndicator(uint8_t red, uint8_t green, uint8_t blue);
    void setupRgbPins(void);
    void setupBlinkInterval(unsigned int time);
    void on(Color color);
    void off();
    void glow(Color color, bool blink);
    void glowStatusCountBlocker(RGBStatus status[], int variableCount);
    void glowStatusCount(RGBStatus status[], int variableCount);
    
};

#endif