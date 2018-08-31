// icebox
// getyourbots.com
#include "Arduino.h"
#include "Beep.h"
#include "Config.h"
#include "LCD.h"
#include "MorseTrainer.h"
#include "MorseEncoder.h"

// check if we should start training
boolean MorseTrainer::isEnabled()
{  
  if((digitalRead(MORSE_TRAINER_BUTTON_PIN)) && (!this->_buttonState))
  {
    this->_buttonState = true;    // button was pressed
  }
  else if((!digitalRead(MORSE_TRAINER_BUTTON_PIN)) && (this->_buttonState))
  {    
    Serial.println("released");
    this->_buttonState = false;  // button was just released
    return true;
  }

  return false;  // return false by default
}
// start the training
void MorseTrainer::startTraining(LCD *lcd)
{
  // first things first, reset LCD
  lcd->resetLcd();
  String line;

  // letters first
  // ABCDEFGHIJKLMNOPQRSTUVWXYZ
  // --------------------------------------------
  line = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  this->_train(line, lcd);    // train the letters

  // sleep for dotpulse * 3 millis, between sets of characters
  delay(this->_dotPulse * 3);    

  // check if we're out
  if(this->_get_out)
  {
    this->_get_out = false;    // make it false (ready for next run)
    return;    // button was repressed
  }

  // digits  
  // 1234567890
  // --------------------------------------------
  line = "1234567890";
  this->_train(line, lcd);    // train the digits
  
  this->_get_out = false;      // re-init _get_out  -  make it false
}

// train string
void MorseTrainer::_train(String str, LCD *lcd)
{
  // iterate through each char of the string
  for(int i = 0; i < str.length(); i++)
  {
    // in case get_out is set (button was pressed while training was working)
    if(this->_get_out)
    {
      break;    
    }
    String current_str = this->generate_next_string(str, i);

    // generated in playCharacter method
    //char current_char = current_str.charAt(0);  // get first char (which is the one we use now)

    // play character (train)
    this->playCharacter(current_str, lcd);
  }

  // when everything's done, reset the LCD again
  lcd->resetLcd();    // reset LCD
}

// play/train current character
void MorseTrainer::playCharacter(String str, LCD *lcd)
{
  // RESET LCD
  lcd->resetLcd();      // reset the LCD
  lcd->setCursorPosition(0, 0);    // move to position 0,0 

    char c = str.charAt(0);      // get the character we'll train

  // print some info to serial, if debug enabled
  if(SERIAL_DEBUG)
  {
    Serial.println(c);
    Serial.println(str);
  }

  int dp2 = this->_dotPulse * 2;

  // print char and strying
  lcd->printCharacter(c);
  lcd->setCursorPosition(1, 0);    // move to 2nd line
  lcd->printString(str);
  this->_delay(dp2);

  // get out check
  if(this->_get_out)
  {
    return;
  }

  lcd->setCursorPosition(0, 0);    // move cursor to beginning

  // flick the character two times
  for(int i = 0; i < 2; i++)
  {
    // get out check  (two are enough here)
    if(this->_get_out)
    {
      return;
    }
    
    lcd->printString(String(' '));
    lcd->setCursorPosition(0, 0);    // move cursor to beginning
    this->_delay(dp2);

    lcd->printString(String(c));     
    this->_delay(dp2);
    
    // get out check  (two are enough here)
    if(this->_get_out)
    {
      return;
    }
  }

  // clear it once again
  int cur_column = 0;
  lcd->setCursorPosition(0, cur_column);
  lcd->printString(String(' '));
  lcd->setCursorPosition(0, cur_column);
  // we're clear here, and ready to print the morse code
  // ----------------------------------------------------

  // encode the character to morse
  // can't be more than 10 for 1 letter, actually max is 5
  int morseBuffer[10];
  MorseEncoder me;
  int length = me.encodeToMorse(String(c), morseBuffer);

  // play morse code
  for(int i = 0; i < length; i++)
  {
    // check if button was enabled again, if it was, just get out
    // ----------------------------------------------------------
    if(this->_get_out)
    {
      return;      // go, go go !
    }
    
    // can be either 0 or 1 (dot or line)
    switch(morseBuffer[i])
    {
    case 0:
      if(SERIAL_DEBUG)
      {
        Serial.print(".");
      }
      lcd->printString(String('.'));
      this->beep_up();
      this->_delay(this->_dotPulse);     // beep one dot time, IMCR #1
      this->beep_down();
      // lcd->removeLastChar();     // delete char
      this->_delay(this->_dotPulse);     // sleep for a dot time period
      //lcd->printCharacter(oc, true);        // print it back
      break;

    case 1:                    
      if(SERIAL_DEBUG)
      {
        Serial.print("_");
      }
      lcd->printString(String('_'));
      this->beep_up();
      this->_delay(this->_dotPulse * 3); // beep dot time * 3, IMCR #2
      this->beep_down();
      //lcd->removeLastChar();     // delete char
      this->_delay(this->_dotPulse);     // sleep for a dot time period
      // lcd->printCharacter(oc, true);        // print it back
      break;
    }

    cur_column += 1;                        // increase column
    lcd->setCursorPosition(0, cur_column);  // set cursor position
  }

  delay(dp2);          // let's sleep for double pulse time
}

// generates the next string for the display
// html 'marquee' style
String MorseTrainer::generate_next_string(String str, int current_index)
{
  // ABCDEFGHIJKLMNOPR
  String new_str;

  int end_index = current_index + 16;
  int str_l = str.length();    // get string length

    if(end_index < str_l)
  {
    new_str = str.substring(current_index, end_index);
  }
  else
  {
    end_index = (str_l - current_index) + current_index;
    new_str = str.substring(current_index, end_index);
  }

  return new_str;    // return new generated string
}
// set dot pulse
void MorseTrainer::setDotPulse(int new_pulse)
{
  this->_dotPulse = new_pulse;
}
// beep up
void MorseTrainer::beep_up()
{
  Beep b;
  b.beep_up();
  digitalWrite(LED_PIN, HIGH);    // disable led
}
// beep down
void MorseTrainer::beep_down()
{
  Beep b;
  b.beep_down();
  digitalWrite(LED_PIN, LOW);    // disable led
}
// private delay methods, which also keeps track of button
void MorseTrainer::_delay(int milli_seconds)
{
  int cur_s = 0;
  // iterate until we slept enough
  while(cur_s < milli_seconds)
  {
    if(this->isEnabled())
    {
      this->_get_out = true;  // set it to true
      break;                  // we're on baby
    }
    delay(1);
    cur_s += 1;    // increase with 10 millis
  }
}

