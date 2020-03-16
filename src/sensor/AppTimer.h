#ifndef AppTimer_h
#define AppTImer_h
#include <Arduino.h>

struct AppTimer
{
    private:    
        static AppTimer *instance;
	   
        AppTimer(){
            manualSwPressedTimeInstance = 0;
        }

    public:

        long int manualSwPressedTimeInstance;

        static AppTimer* shared(){
            if(instance == NULL){
                instance = new AppTimer();
            }
            return instance;
        }
};


#endif