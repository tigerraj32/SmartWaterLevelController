#include "Color.h"

Color::Color(long rgb){
    {
    //Serial.println(((rgb>>16) & 0xFF));
    hex = rgb;
    red   =  ((rgb>>16) & 0xFF);
    green =  ((rgb>>8) & 0xFF);
    blue  =  ((rgb) & 0xFF);
    };
}