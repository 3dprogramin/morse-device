// icebox
// getyourbots.com

#ifndef Beep_h
#define Beep_h

#include "Arduino.h"
#include "Config.h"

class Beep
{
  public:
    void beep_up();        // starts beeping
    void beep_down();      // stops beeping
   private:
     int _pin;             // 'control' pin for buzzer
};
#endif
