#include <Arduino.h>

#ifndef IOconfig_h
#define IOconfig_h


#define ON HIGH
#define OFF LOW
#define TRUE  HIGH
#define FALSE LOW

enum Input {
	//Manual Switch to turn automatic and manual config
	manualSw = 2,
	//Top tank pin config
	t1 = 8,
	t2 = 7,
	t3 = 6,

	//Bottom tank pin config
	b1 = 5,
	b2 = 4,
	b3 = 3
};


enum Output {
	motor = 12,
	buzzer = 13,
	R = 11,
	G = 10, 
	B = 9 
};



#endif
