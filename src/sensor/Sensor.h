#pragma once
#include <Arduino.h>

#ifndef Sensor_h
#define Sensor_h


enum WaterStatus {
    empty = 7,
    level1 = 3,
    level2 = 1,
    level3 = 0
};

enum MotorMode {
    automatic = 1,
    manualOnAutoOff  = 2,
    manualOnManualOff = 3
};

class Sensor
{

private:
    static Sensor *instance;
	Sensor();

public:
    
    bool motor;
  
  
    bool t1;
    bool t2;
    bool t3;
    
    bool b1;
    bool b2;
    bool b3;

    int manualSwPressCount;
    bool isManualSwPressed;

    byte tankSequence ;
    byte topTankSequence, bottomTankSequence;
    MotorMode mode;
    
    static Sensor* shared();
    void inputConfig();
    void outputConfig();
    void send(int pin, bool value);
    
    bool isBottomTankEmpty();
    bool readTankStatus();
    void checkTopTank();
    void checkBottomTank();
    void toString();
  
};
#endif
