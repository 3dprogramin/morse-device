// icebox
// getyourbots.com

#include "Arduino.h"
#include "Beep.h"
#include "MorseEncoder.h"
#include "MorseDecoderCharacter.h"
#include "Config.h"
#include "LCD.h"

// check if we're ready (buttonSide) to encode
boolean MorseEncoder::isEnabled()
{
  if((digitalRead(REPLAY_BUTTON_PIN)) && (!this->_buttonState))
  {
    this->_buttonState = true;    // button was pressed
  }
  else if((!digitalRead(REPLAY_BUTTON_PIN)) && (this->_buttonState))
  {
    this->_buttonState = false;  // button was just released
    return true;
  }

  return false;  // return false by default
}

// encode string to morse array - pointer function
int MorseEncoder::encodeToMorse(String x, int *morseBuffer)
{
   // 0 - dot
   // 1 - dash
   // 2 - space
   int length = 0;
   
   for(int i = 0; i < x.length() + 1; i++)
   {
     char c = x.charAt(i);
     int letterCode[MAX_ENCODER_BUFFER];      // keeps track of current letter code  
     
     int l_length = this->morseFromChar(c, letterCode);  // generate morse from character

     if((morseBuffer[length - 1] == 2) && (letterCode[0]) == 3)
     {
       // WE HAVE A SPACE HERE - END OF WORD
       // REMOVE THE LAST END OF CHARACTER, AND REPLACE IT WITH 3 (we don't need end of character
       // anymore, if it's end of word)
       // ----------------------------------------------------------------------------------------
       morseBuffer[length - 1] = 3;    // replace it with 3
     }
     else
     {
       // we have a normal character here
       // --------------------------------
       for(int j = 0; j < l_length; j++)
       {
         morseBuffer[length] = letterCode[j];    // save morse (code) into array
         length += 1;    // increase length
       }
     }
   }
   /*
   // check if it's ending with 2, replace with 3
   if(morseBuffer[length - 1] == 2)
   {
     morseBuffer[length - 1] = 3;
   }
   */
   
   return length;    // return the length of the morseBuffer
}
// play the morse array
// we get the point of the lcd, and use it to clear the screen, and put
// we get the controller to check for a new potentiometer value
// the characters as they're played
void MorseEncoder::playMorse(int morseArray[], int length, LCD *lcd, String originalText)
{
  // WE WANT TO CLEAR THE DISPLAY, AND RE-PRINT THE CHARACTERS
  // AS THEY'RE PLAYED (by the buzzer and led)
  // ----------------------------------------------------------
  // get the buffer, in case the button is re-pressed, to print 
  // all the text back, otherwise, it would pause where it left
  String lcd_buffer = lcd->getBuffer();
  int line = lcd->getCurrentLine();
  int column = lcd->getCurrentColumn();
  // clear the LCD, without resetting the buffer
  lcd->resetLcd(false);
  int counter = 0;        // need a counter to print the original text
  
  
  if(SERIAL_DEBUG)
  {
    Serial.println("MORSE CODE ARRAY (morse)");
    Serial.println("---------------------------------");
  }
  
  // print first character to display
  char oc = originalText.charAt(counter);      // get current character that's gonna be played
  lcd->printCharacter(oc, true);                      // print it to screen
  counter += 1;    // increase counter
  
  for(int i = 0; i < length; i++)
  { 
    // check if button was re-pressed
    if(this->_get_out)
    {
      this->_get_out = false;
      lcd->resetLcd(false);     // reset lcd
      lcd->setBuffer(lcd_buffer);  // set buffer
      lcd->switchToNormal();      // print buffer
      lcd->setCursorPosition(line, column);    // restore previous line:column
      return;      // return
    }
    
    int ce = morseArray[i];        // get current element
    switch(ce)
    {
      case 0:                      // it's a DOT
        if(SERIAL_DEBUG)
        {
          Serial.print(".");
        }
        this->beep_up();
        this->_delay(this->_dotPulse);     // beep one dot time, IMCR #1
        this->beep_down();
       // lcd->removeLastChar();     // delete char
        this->_delay(this->_dotPulse);     // sleep for a dot time period
        //lcd->printCharacter(oc, true);        // print it back
        break;
        
      case 1:                      // it's a DASH
        if(SERIAL_DEBUG)
        {
          Serial.print("_");
        }
        this->beep_up();
        this->_delay(this->_dotPulse * 3); // beep dot time * 3, IMCR #2
        this->beep_down();
        //lcd->removeLastChar();     // delete char
        this->_delay(this->_dotPulse);     // sleep for a dot time period
       // lcd->printCharacter(oc, true);        // print it back
        break;
        
      case 2:                      // end of letter
        if(SERIAL_DEBUG)
        {
          Serial.print(" ");
        }

        this->_delay(this->_dotPulse * 3); // sleep for 3 * dot time period
        
        // just a double check
        if(counter < originalText.length()){         // check if we still have text to go
          lcd->printCharacter(originalText.charAt(counter), true); 
          counter += 1;                          // increase counter
        }
        break;
        
      case 3:                      // end of word
        if(SERIAL_DEBUG)
        {
          Serial.print("   ");
        }
        lcd->printCharacter(' ', true);               // display to LCD
        counter += 1;                                // increase counter
        this->_delay(this->_dotPulse * 7); // sleep for 7 * dot time period     
        if(counter < originalText.length()){         // check if we still have text to go
          lcd->printCharacter(originalText.charAt(counter), true); 
        }
        counter += 1;
        break;
    }
  }
  
  if(SERIAL_DEBUG)
  {
    Serial.println("");
    Serial.println("---------------------------------");
  }
  
  // check for get_out, just in case
  if(this->_get_out)
  {
    this->_get_out = false;
    lcd->resetLcd(false);     // reset lcd
    lcd->setBuffer(lcd_buffer);  // set buffer
    lcd->switchToNormal();      // print buffer
    lcd->setCursorPosition(line, column);    // restore previous line:column
  }
}

// create morse array (dot, dash or space) from character (letter/digit) - pointer function
// 0 - dot
// 1 - dash
// 2 - end of letter
// 3 - space
int MorseEncoder::morseFromChar(char c, int *lc)
{   
    if(c == 'A')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 2;
      return 3;
    }
    else if(c == 'B')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'C')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'D')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'E')
    {
      lc[0] = 0;
      lc[1] = 2;
      return 2;
    }
    else if(c == 'F')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'G')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'H')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'I')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 2;
      return 3;
    }
    else if(c == 'J')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'K')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'L')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'M')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 2;
      return 3;
    }
    else if(c == 'N')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 2;
      return 3;
    }
    else if(c == 'O')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'P')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'Q')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 1;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'R')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'S')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'T')
    {
      lc[0] = 1;
      lc[1] = 2;
      return 2;
    }
    else if(c == 'U')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'V')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 1;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'W')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 2;
      return 4;
    }
    else if(c == 'X')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 1;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'Y')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 2;
      return 5;
    }
    else if(c == 'Z')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 2;
      return 5;
    }
    
    else if(c == '1')
    {
      lc[0] = 0;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 1;
      lc[5] = 2;
      return 6;
    }
    else if(c == '2')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 1;
      lc[5] = 2;
      return 6;
    }
    else if(c == '3')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 1;
      lc[4] = 1;
      lc[5] = 2;
      return 6;
    }
    else if(c == '4')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 1;
      lc[5] = 2;
      return 6;
    }
    else if(c == '5')
    {
      lc[0] = 0;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 0;
      lc[5] = 2;
      return 6;
    }
    else if(c == '6')
    {
      lc[0] = 1;
      lc[1] = 0;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 0;
      lc[5] = 2;
      return 6;
    }
    else if(c == '7')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 0;
      lc[3] = 0;
      lc[4] = 0;
      lc[5] = 2;
      return 6;
    }
    else if(c == '8')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 0;
      lc[4] = 0;
      lc[5] = 2;
      return 6;
    }
    else if(c == '9')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 0;
      lc[5] = 2;
      return 6;
    }
    else if(c == '0')
    {
      lc[0] = 1;
      lc[1] = 1;
      lc[2] = 1;
      lc[3] = 1;
      lc[4] = 1;
      lc[5] = 2;
      return 6;
    }
    
    else if(c == ' ')
    {
      lc[0] = 3;      // space
      return 1;
    }
    
    else
    {
      return 0;    // something else
    }
}
// beep up
void MorseEncoder::beep_up()
{
  Beep b;
  b.beep_up();
  digitalWrite(LED_PIN, HIGH);    // disable led
}
// beep down
void MorseEncoder::beep_down()
{
  Beep b;
  b.beep_down();
  digitalWrite(LED_PIN, LOW);    // disable led
}
// set dot pulse
void MorseEncoder::setDotPulse(int new_pulse)
{
  this->_dotPulse = new_pulse;
}
// tells is button is currently pressed
boolean MorseEncoder::isButtonPressed()
{
  return this->_buttonState;
}

// private delay methods, which also keeps track of button
void MorseEncoder::_delay(int milli_seconds)
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
