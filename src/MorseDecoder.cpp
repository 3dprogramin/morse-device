// icebox
// getyourbots.com

#include "Arduino.h"
#include "Beep.h"
#include "MorseDecoder.h"
#include "MorseButton.h"
#include "Config.h"

void MorseDecoder::initMorseButton()
{
  // init morse button
  this->_morseButton.init();
}

// Construct/generate character
char MorseDecoder::getCharacter()
{
    char dot_or_dash = this->dotOrDash();

    // it's a dot or dash
    // ----------------------
    if(dot_or_dash != '$')
    {
      // RESETS THE COUNTER IF THE TIME PASSED FROM LAST "NOTE" IS TOO BIG
      // DISABLED FOR NOW, FOR HUMAN TYPING
      // IF IT WAS A ROBOT, DEFINETLY ENABLED BACK
      // ARDUINOBOOKMARK
      // -----------------------
      /*int mils = millis() - time_now;
      if(mils > this->_dotPulse * 3)      // if it took too much time, reset the buffer
      {
        mc.reset();    // reset morsecharacter obj
      }*/
      
      switch(dot_or_dash)
      {
        case '.':
          if(SERIAL_DEBUG)
          {
            Serial.print(".");    // print to serial
          }
          this->_morseCharacter.addDot();          // add dot to mc object
          break;
        case '_':
          if(SERIAL_DEBUG)
          {
            Serial.print("_");    // print to serial
          }
          this->_morseCharacter.addDash();         // add dash
          break;
       }
    }
    else
    {
      // NOT a dot or dash
      // -----------------
      
      // check if it's end of buffer
      int millis_di = millis() - this->_currentTime;  // compare it with the last time since btn was released
                  
      // it's a big pulse, 3 times bigger than the dot
      // which means (by the international morse code rule #4)
      // that it's an end of letter
      // IMCR rule #3
      if((millis_di > (this->_dotPulse * 3)) && (this->_morseCharacter.bufferNotEmpty()) && (!this->_buttonState))          // too big of a space delay, it's a letter
      {      
          char real_character = this->_morseCharacter.generate();  // generate the real character from dots and dashes  
          this->resetMorseBuffer();             // reset the buffer
          return real_character;                   // return the real character
      }
    }
    
    // we have here a maximum number of (dots & dashes, which is 5)
    // without waiting for anything, generate the character and return it
    if(this->_morseCharacter.maxSize())
    {
      char real_character = this->_morseCharacter.generate();  // generate the real character from dots and dashes  
      this->resetMorseBuffer();             // reset the buffer
      return real_character;                   // return the real character
    }
    
    return '$';
}
// get dot or dash
char MorseDecoder::dotOrDash()
{
    char morse_value = '$';    // this is our None value
    
    // check if the button is pressed
    if((this->_morseButton.isPressed()) && (!this->_buttonState))       
    {        
        // BUTTON PRESSED
        // --------------
        this->buttonPressed();    // set buttonState, etc
        return '$';               // return nothing (just getting interesting actually)
    }
    else if((!this->_morseButton.isPressed()) && (this->_buttonState))    // if button was just released
    {
        // BUTTON RELEASED
        // --------------
        this->buttonReleased();    // handle things because it got disabled      
    }
    else
    {
        return '$';      // return None
    }
        
    // calculate the amount of time the button was pressed
    // ----------------------------------------------------
    int time_diff = millis() - this->_currentTimePressed;     

    // IMCR - international morse code rule #1
    if(time_diff <= this->_dotPulse)
    {
      // if it were a robot, it the dot would be probably "exact" all the time
      // since we're humans, and this is a micro-controller, I've put a check
      // in case the diff is too low, to skip it
      if(time_diff > this->_dotPulse / 10)      
      {
        morse_value = '.';      // it's a dot
      }
      else
      {
          // only if machine
          // morse_value = '.';
      }
    }
    // rule #2
    else if((time_diff > this->_dotPulse) && (time_diff <= this->_dotPulse * 3))
    {
      morse_value = '_';
    }
    else
    {
      // for simplicity (because we're humans at the end)
      // it will be considered a dash, even if you keep pressing (eg. for 5 minutes)
      // the button
      morse_value = '_';
      //return '$';
    }
    
    return morse_value;    // return morse value
}

// Reset the morse character
void MorseDecoder::resetMorseBuffer()
{
  this->_morseCharacter.reset();
}

// morse button was pressed
void MorseDecoder::buttonPressed()
{
  this->beep_up();                 // enable beeping (and led)
  this->_currentTimePressed = millis();    // set the current time (pressed time)
  this->_buttonState = true;      // _set buttonState to true
}
// morse button was released
void MorseDecoder::buttonReleased()
{
  this->beep_down();            // disable beeping
  this->_buttonState = false;
  this->_currentTime = millis();    // set currentTime (will help in knowing where to stop)
}

// start beeping
void MorseDecoder::beep_up()
{
  Beep b;
  b.beep_up();
  digitalWrite(LED_PIN, HIGH);    // enable led as well
}
// stop beeping
void MorseDecoder::beep_down()
{
  Beep b;
  b.beep_down();
  digitalWrite(LED_PIN, LOW);    // disable led
}
// last update (when the button was last time released)
int MorseDecoder::lastUpdate()
{
  return this->_currentTime;
}
// set the dot pulse
void MorseDecoder::setDotPulse(int new_pulse)
{
  this->_dotPulse = new_pulse;
}
// tells if button is pressed
boolean MorseDecoder::isButtonPressed()
{
  return this->_buttonState;
}
