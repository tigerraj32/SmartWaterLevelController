#ifndef RGBStatus_h
#define RGBStatus_h

#include "Color.h"
#include <Arduino.h>

struct RGBStatus
{

public:    
    Color color  = Color(0x000000);
    int displayCount = 1;
    RGBStatus(Color c);
};



#endif