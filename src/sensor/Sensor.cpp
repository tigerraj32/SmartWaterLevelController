#include "Sensor.h"
#include "../input/IOConfig.h"
#include "../sensor/AppTimer.h"
#include "../sensor/Constant.h"

Sensor::Sensor(){
    motor = LOW;

    t1 = LOW;
    t2= LOW;
    t3 = LOW;

    b1 = LOW;
    b2= LOW;
    b3 = LOW;

    tankSequence = 0xff;
    topTankSequence = 0x00;
    bottomTankSequence = 0x00;
    manualSwPressCount = 0;
    isManualSwPressed = FALSE;
    mode = MotorMode::automatic;
}


Sensor *Sensor::shared(){
    if(instance == NULL){
        instance = new Sensor();
    }
    return instance;
}
void Sensor:: outputConfig(){
        pinMode(Output::buzzer, OUTPUT);
        pinMode(Output::motor, OUTPUT);

}

void Sensor:: send(int pin, bool value){
        digitalWrite(pin, value);    
    }

void Sensor:: inputConfig(){
        
        //put external pull up registor. Interal not working as expected.
        pinMode(manualSw, INPUT_PULLUP);
        
        pinMode(t1, INPUT_PULLUP);
        pinMode(t2, INPUT_PULLUP);
        pinMode(t3, INPUT_PULLUP);

        pinMode(b1, INPUT_PULLUP);
        pinMode(b2, INPUT_PULLUP);
        pinMode(b3, INPUT_PULLUP);
    }

void Sensor::checkTopTank(){
    Sensor::shared()->t1  = digitalRead(Input::t1);
    Sensor::shared()->t2  = digitalRead(Input::t2);
    Sensor::shared()->t3  = digitalRead(Input::t3);
    
    byte value = 0x00;
    value = (Sensor::shared()->t1);
    value = (value << 1 | Sensor::shared()->t2) ;
    value = (value << 1 | Sensor::shared()->t3) ;
    
    topTankSequence = value;
}

void Sensor::checkBottomTank(){
    Sensor::shared()->b1 = digitalRead(Input::b1);
    Sensor::shared()->b2 = digitalRead(Input::b2);
    Sensor::shared()->b3 = digitalRead(Input::b3);

    byte value = 0x00;
    value = (Sensor::shared()->b1);
    value = (value << 1 | Sensor::shared()->b2) ;
    value = (value << 1 | Sensor::shared()->b3) ;
    
    bottomTankSequence = value;
}


bool Sensor::readTankStatus(){

    checkTopTank();
    checkBottomTank();

    byte value = 0x00;
    //top tank status 
    value = (Sensor::shared()->t1);
    value = (value << 1 | Sensor::shared()->t2) ;
    value = (value << 1 | Sensor::shared()->t3) ;

    //bottom tank status
    value = (value << 1 | Sensor::shared()->b1) ;
    value = (value << 1 | Sensor::shared()->b2) ;
    value = (value << 1 | Sensor::shared()->b3) ;
   
    bool flag = Sensor::shared()->tankSequence  == value ? FALSE : TRUE;
    Sensor::shared()->tankSequence =  value;
    return  flag;
}

bool Sensor::isBottomTankEmpty(){
    return Sensor::shared()->b1;
}

void Sensor::toString(){
    Serial.print(millis());
    Serial.print(" - ");
    Serial.print(Sensor::shared()->tankSequence, BIN);
    Serial.print(" -> in Decimaal ");
    Serial.println(Sensor::shared()->tankSequence, DEC);
}

