// icebox/
// getyourbots.com

#ifndef MorseTrainer_h

#define MorseTrainer_h

#include "Arduino.h"
#include "Config.h"
#include "LCD.h"

class MorseTrainer
{
  public:
    boolean isEnabled();	// check if it's enabled
    void startTraining(LCD *lcd);	// start the training
    void _train(String str, LCD *lcd);  // train string
    void setDotPulse(int new_pulse);    // set dot pulse value
  private:
    String  generate_next_string(String str, int current_index);    // generates the next string
    void playCharacter(String current_str, LCD *lcd);            // play character (first char from string)
    void beep_up();               // beep up 
    void beep_down();             // beep down
    void _delay(int milli_seconds);     // custom sleep function
    boolean _buttonState = false;      // keeps track of button state
    int _dotPulse;                // dot pulse
    boolean _get_out = false;    // keeps track of button if it was re-pressed while training was going on
};

#endif
