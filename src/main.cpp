#include <Arduino.h>
#include "output/RGBIndicator.h"
#include "input/IOConfig.h"
#include <SimpleTimer.h>
#include "sensor/Sensor.h"
#include "sensor/AppTimer.h"
#include "sensor/Constant.h"


SimpleTimer appTimer;
Sensor *Sensor::instance = NULL;
AppTimer *AppTimer::instance = NULL;

auto rgbIndicator = new RGBIndicator(R,G,B);
auto topTankStatus = RGBStatus(Color(red));
auto bottomTankStatus = RGBStatus(Color(green));
auto motorMode = RGBStatus(Color(blue));

#define FULL LOW
#define EMPTY HIGH

void rgbHandler(){
  // //Read input signal first
  //topTankStatus.displayCount = 5;
  //bottomTankStatus.displayCount = 3;
  //motorMode.displayCount = 1;
  RGBStatus statusArray[] = {topTankStatus, bottomTankStatus, motorMode};

  rgbIndicator->glowStatusCount(statusArray, 3);
}

void readTank(){
 if(Sensor::shared()->readTankStatus()){ 
    //when there is new value available.

    //reset rgb counter status.
    rgbIndicator->reset = true;

   //set rgb status for top tank
    switch (Sensor::shared()->topTankSequence){
      case WaterStatus::empty:
        Serial.println("Empty: 1");
        topTankStatus.displayCount = 1;
        break;

      case WaterStatus::level1:
        Serial.println("l1: 2");
        topTankStatus.displayCount = 2;
        break;

      case WaterStatus::level2:
        Serial.println("l2: 3");
        topTankStatus.displayCount = 3;
        break;

      case WaterStatus::level3:
        Serial.println("l3: 4");
        topTankStatus.displayCount = 4;
        break;
      default:
        Serial.println("Invalid case");
        break;
    }
    //set rgb status for top tank
    switch (Sensor::shared()->bottomTankSequence){
      case WaterStatus::empty:
        bottomTankStatus.displayCount = 1;
        break;

      case WaterStatus::level1:
        bottomTankStatus.displayCount = 2;
        break;

      case WaterStatus::level2:
        bottomTankStatus.displayCount = 3;
        break;

      case WaterStatus::level3:
        bottomTankStatus.displayCount = 4;
        break;
      default:
        Serial.println("Invalid tank read case");
        break;
    }
  
  }

  Sensor::shared()->toString();
}

void readManualSwitch(){
 if (((millis() - AppTimer::shared()->manualSwPressedTimeInstance) >Constant::Timer::stable) 
    && 
    AppTimer::shared()->manualSwPressedTimeInstance > 0){
    
        switch (Sensor::shared()->manualSwPressCount)
        {
        case MotorMode::automatic:
          Sensor::shared()->mode = MotorMode::automatic;
          Serial.println("----------------- Mode Switch:- automatic mode ----------------");
          motorMode.displayCount = 1;
          break;

        case MotorMode::manualOnAutoOff:
            Sensor::shared()->mode = MotorMode::manualOnAutoOff;
            Serial.println("----------------- Mode Switch:- manual on auto off mode ----------------");
          motorMode.displayCount = 2;
            break;

        case MotorMode::manualOnManualOff:
            Sensor::shared()->mode = MotorMode::manualOnManualOff;
            Serial.println("----------------- Mode Switch:- manual on manual off mode ----------------");
            motorMode.displayCount = 3;
            break;            

        default:
            break;
        }

      Sensor::shared()->manualSwPressCount = 0;
      AppTimer::shared()->manualSwPressedTimeInstance = 0;
       
  } 
}

void controlMotor(){
  if((Sensor::shared()->mode == MotorMode::automatic) || (Sensor::shared()->mode == MotorMode::manualOnAutoOff)){
    Serial.println("motor running in automatic mode");

    if (Sensor::shared()->mode == MotorMode::manualOnAutoOff){
      Sensor::shared()->motor = ON;
      Sensor::shared()->send(Output::motor, Sensor::shared()->motor);
      //after turning on motor, need to set config to auto otherwise  motor will on and off.
      Sensor::shared()->mode = MotorMode::automatic;
      //motorMode.displayCount = 1;
    }
    

    if (Sensor::shared()->isBottomTankEmpty() == FALSE){
      
      Serial.println("Bottom tank have water");
      if (Sensor::shared()->motor == OFF){
        Sensor::shared()->motor = (Sensor::shared()->t1 & Sensor::shared()->t2 & Sensor::shared()->t3) &
                                (
                                  (~Sensor::shared()->b1 & ~Sensor::shared()->b2 & ~Sensor::shared()->b3) |
                                  (~Sensor::shared()->b1 & ~Sensor::shared()->b2 & Sensor::shared()->b3)  |
                                  (~Sensor::shared()->b1 & Sensor::shared()->b2 & Sensor::shared()->b3) 
                                 );

        Sensor::shared()->send(Output::motor,Sensor::shared()->motor);
      }else{
        //keep running motor until top tank is full once  motor is started.
        if ((Sensor::shared()->t1 == FULL) & (Sensor::shared()->t2 == FULL) &(Sensor::shared()->t3 == FULL) ){
          Sensor::shared()->motor = OFF;
          Sensor::shared()->send(Output::motor,Sensor::shared()->motor);
        }
        
      }

      Serial.print("Motor Running status:- ");
      Serial.println(Sensor::shared()->motor);

    }else{
      //Bottom tank empty
      Serial.println("Bottom tank empty");
      Sensor::shared()->motor = OFF;
      Sensor::shared()->send(Output::motor, Sensor::shared()->motor);
    }
    
  }else{
    Serial.println("motor running in manual on manual off mode");
    Sensor::shared()->motor = ON;
    Sensor::shared()->send(Output::motor, Sensor::shared()->motor);
  }
}

void manualHandler(){
  if ((Sensor::shared()->isManualSwPressed == FALSE) ){
    Sensor::shared()->isManualSwPressed = TRUE;
    Sensor::shared()->manualSwPressCount = Sensor::shared()->manualSwPressCount + 1;
    Serial.print("Manual Switch Pressed:-");
    Serial.println(Sensor::shared()->manualSwPressCount);
  }
  AppTimer::shared()->manualSwPressedTimeInstance = millis();
}

void setup() {
  //Configure Serial communication baud rate
  Serial.begin(9600);

 //Setup sendor pins
  Sensor::shared()->inputConfig();
  Sensor::shared()->outputConfig();
  
  //on startup set config mode to automatic.
  Sensor::shared()->mode = MotorMode::automatic;
  //On startup turnoff motor
  Sensor::shared()->send(Output::motor, OFF);

  //Configure RGB led
  rgbIndicator->setupRgbPins(); //io setup
  rgbIndicator->setupBlinkInterval(300); //blink interval setup
    
  //setup time triggered loop.
  appTimer.setInterval(200, rgbHandler);  
  appTimer.setInterval(1000, readTank); 
  appTimer.setInterval(Constant::Timer::stable, readManualSwitch);
  appTimer.setInterval(Constant::Timer::motor, controlMotor);
  attachInterrupt(digitalPinToInterrupt(manualSw), manualHandler, FALLING );
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //rgbIndicator->glow(Color(red), true);   
  //Stage 1:  Determine manual or automatic

  if(Sensor::shared()->isManualSwPressed == TRUE){
    Sensor::shared()->send(Output::buzzer, TRUE);
    if ((millis() - AppTimer::shared()->manualSwPressedTimeInstance) > Constant::Timer::debounce){
        Sensor::shared()->isManualSwPressed = FALSE;
        Sensor::shared()->send(Output::buzzer, FALSE);
      }
  }
  //run timer
  appTimer.run();
}