// icebox/
// getyourbots.com

#ifndef MorseDecoder_h

#define MorseDecoder_h

#include "Arduino.h"
#include "MorseDecoderCharacter.h"
#include "Config.h"
#include "MorseButton.h"

class MorseDecoder
{
  public:
    char getCharacter();          // creates the character (for the controller)
    int lastUpdate();             // last update (when the button was last time released)
    void resetMorseBuffer();      // resets the morse buffer (for a new letter)
    void setDotPulse(int new_pulse);    // set the dot pulse
    boolean isButtonPressed();
    void initMorseButton();
  private:
    void buttonPressed();         // (fires on) morse button was pressed
    void buttonReleased();        // (fires on) morse button was released
    void beep_up();               // beep up 
    void beep_down();             // beep down
    char dotOrDash();             // checks for dot/dash, button pressed/released
    MorseDecoderCharacter _morseCharacter;    // helps in building the character from dots and dashes
    MorseButton _morseButton;                   // morse button obj
    boolean _buttonState = false;      // keep track of button (pressed/released) state
    int _currentTime;                  // last time the button was released
    int _currentTimePressed;           // last time the button was pressed
    int _dotPulse;
};

#endif
