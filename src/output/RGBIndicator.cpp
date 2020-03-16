
#include "RGBIndicator.h"
#include <Arduino.h>

/* RGB indicator constructor */
RGBIndicator::RGBIndicator(uint8_t r, uint8_t g, uint8_t b){
  
  pinRed = r;
  pinGreen = g;
  pinBlue = b;
 
}

/* setup rgb pin as output */
void RGBIndicator::setupRgbPins(){
    
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);
   
}

/* provide custome blink interval */

void RGBIndicator::setupBlinkInterval(unsigned int time){
    interval = time;
}
//glow led with provided color.
void RGBIndicator::on(Color color){

    analogWrite(pinRed,     (color.red));
    analogWrite(pinGreen,   (color.green));
    analogWrite(pinBlue,    (color.blue));
}

//turnoff led.
void RGBIndicator::off(){
    on(Color(0x000000));
}

//blink rgb led to glow with provided color code.
void RGBIndicator::glow(Color color, bool blink){
    unsigned long now = millis();
    unsigned int segment = now % (interval * 2);
    
    if (blink){
        if (interval >= 0  && segment < interval){
            on(color); 
            }else{
                off();
        }
    }else{
        on(color); 
    }
    
}

//current color 
//color count
//isrunning


/*
300-r, 300-off, 300-r,  //red count 2
500-off,
300-g, 300-off, 300-g, 300-g, 300-off, 300-g,  //green  count 4
500-off,
300-b,  //blue count 1
500-off,

*/
void RGBIndicator::glowStatusCountBlocker(RGBStatus status[], int variableCount){

    for ( int i = 0; i < variableCount; i++) {
        auto rgbstatus  = status[i];
        for( int j = 0; j < rgbstatus.displayCount; j++){
            on(rgbstatus.color);
            delay(100);
            off();
            delay(100);
            }
        delay(interval);
    }

}

/*
TODO: Functional implementation is incomplete.
*/
 void RGBIndicator::glowStatusCount(RGBStatus statusArray[],  int variableCount){
   
     if (reset){
         currentCount = 0;
         currentIndex = 0;
         reset = false;
     }

    RGBStatus status = statusArray[currentIndex];
     if (currentCount % 2 == 0 && currentCount < status.displayCount * 2){
        Serial.print(currentIndex);
        Serial.print(" -on- ");
        Serial.println(currentCount);

         on(status.color);
         currentCount += 1;
     }else if (currentCount % 2 == 1 && currentCount < status.displayCount * 2){
          Serial.print(currentIndex);
          Serial.print(" -off- ");
          Serial.println(currentCount);

         off();
         currentCount += 1;
     }else{
         Serial.println(" -+++- ");
         currentIndex += 1;
          currentCount = 0;
         if (currentIndex > (variableCount - 1)){
           Serial.println(" ------ ");
            currentIndex = 0;
           
         }
         
     }
     
     
     

 }


