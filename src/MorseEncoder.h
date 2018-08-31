// icebox/
// getyourbots.com

#ifndef MorseEncoder_h

#define MorseEncoder_h

#include "Arduino.h"
#include "Config.h"
#include "LCD.h"

class MorseEncoder
{
  public:
    boolean isEnabled();          // checks if we're ready to roll
    int encodeToMorse(String txt, int *morseBuffer);   // encodes given string to morse (buzz & led)
    void playMorse(int morseArray[], int length, LCD *lcd, String originalText);      // play morse code (buzz & led)
    void setDotPulse(int new_pulse);    // set the dot pulse
    boolean isButtonPressed();
  private:
    int morseFromChar(char c, int *letterCode);        // takes a char, and translates it into morse code
    void beep_up();               // beep up 
    void beep_down();             // beep down
    void _delay(int milli_seconds);     // custom sleep function
    boolean _buttonState = false;      // keep track of button (pressed/released) state
    int _dotPulse;                // dot pulse
    boolean _get_out = false;     // keeps track of re-push of the button
};

#endif
