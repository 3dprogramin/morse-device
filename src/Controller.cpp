// icebox
// getyourbots.com

#include "Arduino.h"
//#include "LowPower.h"
#include "Beep.h"
#include "Controller.h"
#include "MorseDecoder.h"
#include "MorseEncoder.h"
#include "MorseTrainer.h"
#include "MorseButton.h"
#include "Config.h"

// --------------------------------------------------------------------------------------
// INIT METHODS
// --------------------------------------------------------------------------------------
// Initialize the controller
void Controller::init()
{
  // stuff here and there	
  this->_last_update_time = millis();

  this->initPins();     // init PINs
  this->initMorseDecoder();  // init morse decoder (morse button/capacitive sensor)

  // initialize the LCD
  digitalWrite(LED_PIN, HIGH);    // disable led
  this->_lcd.init();    // init LCD object
  digitalWrite(LED_PIN, LOW);    // disable led

  Serial.begin(9600);   // init serial communication
  if(SERIAL_DEBUG)
  {
    Serial.println("-----------------------------------");      
    Serial.println("STARTED");
    Serial.println("-----------------------------------");
  }
  
  // init the dot pulses
  this->_potentiometerValueChange();    // do it now, before we go into loop
}
// init pins
void Controller::initPins()
{
  // buttons
  // -------
  pinMode(MORSE_TRAINER_BUTTON_PIN, INPUT);
  pinMode(REPLAY_BUTTON_PIN, INPUT);
  pinMode(CLEAR_BUTTON_PIN, INPUT);
  
  // led
  // ---
  pinMode(LED_PIN, OUTPUT);
  // buzzer
  // ------
  pinMode(BUZZER_PIN, OUTPUT);
}

// init morse button
void Controller::initMorseDecoder()
{
  this->_morseDecoder.initMorseButton();
}

// --------------------------------------------------------------------------------------
// LOOPER METHOD
// --------------------------------------------------------------------------------------
// Runs the program
void Controller::looper()
{
  // low power consumtion
  //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER5_OFF, TIMER4_OFF, TIMER3_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART3_OFF, USART2_OFF, USART1_OFF, USART0_OFF, TWI_OFF);
                
  this->_morseSignalDecoder();    // decode morse signal to text
  this->_removeLastCharFromDisplay();    // remove last character from display
  //this->_morseSignalReplay();    // encode text to morse signal
  this->_potentiometerValueChange();    // check for potentiometer value changes
  this->_morseSignalTrainer();              // morse trainer method
}


// --------------------------------------------------------------------------------------
// MAIN PRIVATE METHODS
// --------------------------------------------------------------------------------------
// get the morse character using the morse library (easy as that niggwua)
void Controller::_morseSignalDecoder()
{
  char c = this->_morseDecoder.getCharacter();    // get the morse character

  int v = millis() - this->_morseDecoder.lastUpdate();
    
  boolean diff_ok = v > this->_potentiometerValue * 7;      // based on IMCR #5

  if(c != '$')
  {
    if(SERIAL_DEBUG)
    {
      Serial.println("");
      Serial.print(c);                     // print it on the console
      Serial.println("");
    }
    this->_word += c;      // append character to word
    this->_lcd.addCharacter(c);  // print to display
    this->_space_ok = true;    // space is good now, we just got a char
    this->_last_update_time = millis();
  }
  else if((this->_space_ok) && (diff_ok) && (this->_word.length() > 0))
  {
    // not a normal character
    // but check if it's not end of word
    // ICMR #5
    // prints a space, end of word
    if(SERIAL_DEBUG)
    {
      Serial.println("------------------------");
      Serial.println("Word: " + this->_word);
      Serial.println("------------------------");     
    }
    this->_lcd.addCharacter(' ');       // add a ' ' to display

    this->_word = "";                // reset word
    this->_space_ok = false;            // keep track of space, so we don't do it again
    return; 
  }
}
// Encode the text (from display) to morse (buzz & led) - replay it
void Controller::_morseSignalReplay()
{
  boolean readyToEncode = this->_morseEncoder.isEnabled();      // check if we should start encoding

  if(readyToEncode)
  {
    String LCDtext = this->_lcd.getBuffer();    // get current text from display
    LCDtext.trim();      // trim it, otherwise we get only the ' ' and it's taking it as a letter/word

    if(LCDtext.length() == 0)    // check length
    {
      if(SERIAL_DEBUG)
      {
        Serial.println("No text to encode to morse");
        return;
      }
    }

    int morseBuff[MAX_ENCODER_BUFFER];    // int array, will have the morse code here, once the encodeToMorse finishes

    if(SERIAL_DEBUG)
    {
      String info = "Encoding text to morse: -" + LCDtext + "-";
      Serial.println(info);    // print what we're going to encode
    }

    // encode the text, result is gathered into x[]
    // -------------------------------------------------------------
    int arrLength = this->_morseEncoder.encodeToMorse(LCDtext, morseBuff);       // generate the morse code array

    // IN x WE HAVE THE MORSE CODE, arrLength - it's length
    // ----------------------------------------------------
    if(SERIAL_DEBUG)
    {
      Serial.println("MORSE ENCODED ARRAY (int)");
      Serial.println("----------------");
      for(int i = 0;i < arrLength; i++)
      {
        Serial.print(morseBuff[i]);    // print each character
      }
      Serial.println("");
      Serial.println("----------------");
    }

    // play the morse code with buzzer and led
    // we pass along the lcd as a pointer and we
    // pass the LCDtext for re-priting it back, as it plays
    this->_morseEncoder.playMorse(morseBuff, arrLength, &this->_lcd, LCDtext);
    
    // set cursor to be at right position, it gets mixed after we printed it
    // important! - otherwise, it won't work as expected with the last char
    // (spent bit of time, trying to figure it out, after a long day of programming)
    this->_lcd.setCursor();    
  }
}
// clear last character from display
void Controller::_removeLastCharFromDisplay()
{
  // check if we should remove the last character
  // based on button, buffer (size) of LCD, last char (of buffer) is space
  if(this->checkRemoveLastChar())    
  {
    if(SERIAL_DEBUG)
    {
      Serial.println("last char removed");
    }
    this->_lcd.removeLastChar();    // remove last character
  }
}
// check for potentiometer value changes
void Controller::_potentiometerValueChange()
{
  int pot_val = analogRead(POTENTIOMETER_PIN);    // read the value from the sensor
  int val = 1023 - pot_val;                       // it's reversed when read, map it right (reverse it)
  
  // no point for it to be 0 really ..
  // ---------------------------------
  if(val == 0)
  {
    val = 1;  
  }
  // gonna be -1 if it came here from init
  // check if it's different than what we have
  // -----------------------------------------
  // get the difference since last change
  int diff = this->_potentiometerValue - val;
  if(diff < 0)
  {
     // we have a negative number, still good, just * -1 to get the positive value
     diff = diff * -1;
  }

  // use -3 and + 3 because otherwise we get changes continuosly
  // this makes the range a bit bigger
  // if morse button is pressed, just ignore, we get different values to the potentiometer
  // as well, because of the current
  // could be probably solved, waiting for suggestions on this one
  // same goes for the replay button
  if((diff > POTENTIOMETER_PULSE_MIN_DIFF) && (!this->_morseDecoder.isButtonPressed()) && (!this->_morseEncoder.isButtonPressed()))    
  {
    if(SERIAL_DEBUG)
    {
      String txt = "new dot pulse value: ";
      Serial.println(txt + val);
    }
    this->_potentiometerValue = val;            // set value for current object (we need it here too)    
    
    this->_morseEncoder.setDotPulse(val);      // set for encoder
    this->_morseDecoder.setDotPulse(val);      // set for decoder
    this->_morseTrainer.setDotPulse(val);      // set for trainer
    
    // change the display, to see what we just set
    // ---------------------------------------------------------------------------------------
    this->_lcd.dotValueChanged(val);            // show on the LCD that the dot value changed
  }
  
  // check if we should switch back to normal
  if(this->_lcd.shouldSwitch())
  {
    this->_lcd.switchToNormal();            // switch back to the original lcd details
  }
}
// morse trainer (helps in learning the alphabet and digits in morse code)
void Controller::_morseSignalTrainer()
{
  String l_buffer;
  int line, column;
  // check if we should start the 'training'
  // based on button press
  if(this->_morseTrainer.isEnabled())
  {
    l_buffer = this->_lcd.getBuffer();    // get the buffer locally (it's gonna get changed in obj)
    line = this->_lcd.getCurrentLine();          // get current line #
    column = this->_lcd.getCurrentColumn();      // get current column #
    this->_morseTrainer.startTraining(&this->_lcd);    // start the training
    this->_lcd.setBuffer(l_buffer);    // set the local buffer as original
    this->_lcd.setCursorPosition(line, column);    // set current position
    this->_lcd.switchToNormal();       // switch back to normal
  }
}

// --------------------------------------------------------------------------------------
// AUXILIARY METHODS
// --------------------------------------------------------------------------------------
// Checks if the button was pressed (and released)
// for removing the last character from display
boolean Controller::checkRemoveLastChar()
{
  if((digitalRead(CLEAR_BUTTON_PIN)) && (!this->_removeLastCharBtnState))
  {
    this->_removeLastCharBtnState = true;    // button was pressed
  }
  else if((!digitalRead(CLEAR_BUTTON_PIN)) && (this->_removeLastCharBtnState))
  {
    // if the buffer has only one char (with no space)
    // it won't delete it, because it's a 'special case', threat it here
    boolean special_case = this->_lcd.getBuffer().length() == 1;    // special case
    this->_removeLastCharBtnState = false;  // button was just released
    // if the last character is a space
    // and the buffer is not empty, return true
    return (!this->_space_ok && this->_lcd.bufferNotEmpty()) || special_case;     
  }

  return false;  // return false by default
}
