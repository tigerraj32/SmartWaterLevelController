
#ifndef Color_h
#define Color_h

#include <Arduino.h>

enum Code {
   off = 0x000000,
   white = 0xffffff,
   red = 0xff0000,
   green = 0x00ff00,
   blue = 0x0000ff,
   magenta = 0xff0090,
   purple = 0xf00fff,
   orange = 0xff0f00,
   yellow = 0xf0f000,
   pink = 0xff0f0f,
   brown = 0xA52A2A,
   cyan = 0x00FFFF,
   gray = 0x808080,
   navy = 0x000080
};

struct Color{

public:	
    uint8_t red;
	uint8_t green;
	uint8_t blue;
    byte hex;
    Color(long rgb);
  };

  #endif