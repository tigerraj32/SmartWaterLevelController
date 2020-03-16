# SmartWaterLevelController

## RGBIndicator

**Setup**
- Initialize RGBIndicator with output pins
```c
auto rgbIndicator = new RGBIndicator(11,10,9);

void setup() {
  rgbIndicator->setupRgbPins(); //io setup
}

```

- glow led with specified color. Use function **glow** that take <br> 
input parameter as  color and a flag that will control rgb led to blink or not. 
- if flag is set to true then RGB led will blink in intetval of 500ms.

```c
void loop(){
  rgbIndicator->glow(Color(orange), true);
}
```

Blink interval can be controlled with setter  **setupBlinkInterval**

```c
rgbIndicator->setupBlinkInterval(300); //blink interval setup
```

## RGB Indicator to represent multiple variables.

RGBIndicator is capable of displaying multiple variable status using single RGB led with blink count for each variable.

For eg. 
- variable a = 5; //blink 5 time with color red
- variable b = 3  //blink 3 time with color green
- variable c = 2;  //blink 2 time with color blue

```c
#include <SimpleTimer.h>

SimpleTimer appTimer;

auto a = RGBStatus(Color(red));
auto b = RGBStatus(Color(green));
auto b = RGBStatus(Color(blue));

void rgbHandler(){
  // //Read input signal first
  a.displayCount = 5;
  b.displayCount = 3;
  c.displayCount = 1;
  RGBStatus statusArray[] = {a, b, c};

  rgbIndicator->glowStatusCount(statusArray, 3);
}

void setup(){
    appTimer.setInterval(200, rgbHandler);  
}

void loop(){
    appTimer.run();
}

```



## Debounce Interrupt

Interrupt Service Routine ISR are trigger when ever there is a falling or rising or change (as configured) in interrupt pins. So when we connect switch to interrupt pins it trigger sequence of interrupt signal even for single press. This is because switch being mechanical input device and have debounce effect. To read correctly we will need to configure the interrupt debounce as well.

```c

int manualSwPressCount = 0;

void motorOnInstruction(){
  if ((AppState::shared()->isSetting == FALSE) ){
    AppState::shared()->isSetting = TRUE;
    manualSwPressCount = manualSwPressCount + 1;
    Serial.println(manualSwPressCount);
  }
  AppTimer::shared()->timerSetting = millis();

}

void setup(){
  attachInterrupt(digitalPinToInterrupt(2), motorOnInstruction, FALLING );
}

void loop() {

//Stage 1:  Determine manual or automatic
  if(AppState::shared()->isSetting == TRUE){
    if ((millis() - AppTimer::shared()->timerSetting) > Constant::Timer::debounce){
        AppState::shared()->isSetting = FALSE;
        
      }
  }
     
  //exit from interrupt mode after  there signal is stable for more than 1 sec.
  if (((millis() - AppTimer::shared()->timerSetting) >Constant::Timer::stable) 
    && 
    AppTimer::shared()->timerSetting > 0){
    
      if(manualSwPressCount % 2 == 0){
        AppState::shared()->mode = MotorMode::automatic;
        Serial.println("automatic mode");
          
      }else{
        AppState::shared()->mode = MotorMode::manual;
        Serial.println("manual mode");
      }

      manualSwPressCount = 0;
      AppTimer::shared()->timerSetting = 0;
       
  } 
}


```
