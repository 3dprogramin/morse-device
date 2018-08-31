// icebox
// getyourbots.com

#include "Arduino.h"
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
#include "Config.h"

// Initialize the LCD object, called only once
void LCD::init()
{ 
  // init the line & column variables 
  this->initLcdVars();
  
  this->resetBuffer();    // make buffer String = ""
  this->initLcd();        // init LCD
}
// init lcd vars to start from the beginning
void LCD::initLcdVars()
{
  this->_current_line = 0;
  this->_current_column = -1;
}
// Initialize the LCD
void LCD::initLcd()
{ 
  // init LiquidCrystal
  this->_liquidCrystal.init(0x27, 16, 2);
  this->_liquidCrystal.begin();
  this->_liquidCrystal.backlight();
  this->boot();
}
// boot message for display
void LCD::boot()
{
  this->_liquidCrystal.print("Initializing ...");
  this->_current_line = 1;
  this->setCursor();
  this->_liquidCrystal.print("morsedevice.com");
  delay(1000);
  this->resetLcd();
}
// character was added
// add to buffer + print to LCD
void LCD::addCharacter(char c)
{ 
  this->_buffer += c;
  // before printing anything, check the position, and set the positions
  if((this->_current_line == 1) && (this->_current_column == 15))
  {
    this->resetLcd();
    this->_current_column = 0;  // make current column 0 (because it's reset to -1 first)
    this->setCursor();      // set cursor right
  }
  else if((this->_current_line == 0) && (this->_current_column == 15))
  {
    // move to second line
    this->_current_line = 1;
    this->_current_column = 0;
    this->setCursor();    // set cursor right
  }
  else
  {
    // increase column
    this->_current_column += 1;
    this->setCursor();
  }

  this->printCharacter(c);    // print character to display
  // in case the buffer ends with this words, clear the LCD, just for fun
  if((this->_buffer.endsWith("CLEAR")) ||(this->_buffer.endsWith("STERGE")))
  {
    delay(200);             // delay to print the last char, otherwise will be too fast
    this->resetLcd();       // clear LCD display
  }
  
}
// print character to display
void LCD::printCharacter(char c, boolean change_position)
{
  // check if it's in the pulse view mode
  // -----------------------------------------------------
  if(this->_pulse_view_mode)
  {
    this->switchToNormal();    // switch to normal view
  }
  // -----------------------------------------------------
  
  if(change_position)
  {
    // before printing anything, check the position
    if((this->_current_line == 1) && (this->_current_column == 15))
    {
      this->resetLcd();
      this->_current_column = 0;  // make current column 0 (because it's reset to -1 first)
      this->setCursor();      // set cursor right
    }
    else if((this->_current_line == 0) && (this->_current_column == 15))
    {
      // move to second line
      this->_current_line = 1;
      this->_current_column = 0;
      this->setCursor();    // set cursor right
    }
    else
    {
      // increase column
      this->_current_column += 1;
      this->setCursor();
    }
  }
  
  this->_liquidCrystal.print(c);    // print character to display
}
// set cursor to (this) column and line
void LCD::setCursor()
{
  this->_liquidCrystal.setCursor(this->_current_column, this->_current_line);
}
// clear LCD
void LCD::resetLcd(boolean reset_buffer)
{
  if(SERIAL_DEBUG)
  {
    Serial.println("lcd reset");
  }
  this->initLcdVars();             // reset the LCD display vars
  this->_liquidCrystal.clear();    // clear the LCD display
  if(reset_buffer)
  {
    if(SERIAL_DEBUG)
    {
      Serial.println("buffer reset");
    }
    this->resetBuffer();    // reset buffer
  }
}
// removes the last character from the display
void LCD::removeLastChar(boolean remove_from_buffer)
{ 
  // we're already in the right position
  // position is increased before it's printed, so we're already
  // on the right line:column
  this->printCharacter(' ');              // remove the character that was currently in position

  // clear the last character from the screen
  // there is a 'special case' if the line = 1 and column = 0
  // if we want to go back, we have to change the line and column
  // differently than for the rest
  if((this->_current_line == 1) && (this->_current_column == 0))
  {
    this->_current_line = 0;
    this->_current_column = 15;
    this->setCursor();      // set cursor right
  }
  else if((this->_current_line == 0) && (this->_current_column == -1))      // we're at the beginning
  {
    // PASS
  }
  else if((this->_current_line == 0))                                      // if our line is already 0, just decrease the column
  {
    this->_current_column -= 1;
    this->setCursor();
  }
  else if(this->_current_line == 1)
  {
    this->_current_column -= 1;
    this->setCursor();
  }
  else                                                                    // if line = 1 and column != 0, decrease both
  {
    this->_current_line -= 1;
    this->_current_column -= 1;
    this->setCursor();
  }

  // removes the character from the buffer
  if(remove_from_buffer)
  {
    int l = this->_buffer.length();    // get buffer length
    this->_buffer = this->_buffer.substring(0, l - 1);    // cut last character
  }
}
// make string buffer empty
void LCD::resetBuffer()
{
  this->_buffer = "";    // reset buffer
}
// returns the current buffer
String LCD::getBuffer()
{
  return this->_buffer;
}
// tells if we have something in the buffer - on the display
boolean LCD::bufferNotEmpty()
{
  return this->_buffer.length() > 0;
}
// dot pulse value changed, show it, and keep track of current display
void LCD::dotValueChanged(int value)
{ 
  this->_liquidCrystal.clear();    // clear the LCD display
  
  String txt = "Dot value: " + String(value);
  
  this->_liquidCrystal.setCursor(-1, 0);    // move to beginning of display
  
  // print the text to liquidcrystal
  this->_liquidCrystal.print(txt);
  
  this->_pulse_view_mode = true;                // set the pulse view variable to true
  this->_pulse_view_timestamp = millis();  // set the timestamp
}
// check if it should switch back to original
boolean LCD::shouldSwitch()
{
  if(this->_pulse_view_mode)
  {
    int diff = millis() - this->_pulse_view_timestamp;    // get the time difference since it was switched
    if(diff > LCD_PULSE_VIEW_BEFORE_SWITCH)
    {
      this->_pulse_view_mode = false;
      return true;    // ok, we have to switch it back now
    }
  }
  return false;
}
// switch to normal display if needed
void LCD::switchToNormal()
{
  this->_liquidCrystal.clear();    // clear the LCD display
  this->_liquidCrystal.setCursor(-1, 0);    // move to beginning of display
  int b_length = this->_buffer.length();
  
  // check the length of the buffer, and print it on the display
  // -----------------------------------------------------------
  if(b_length == 0)
  {
    // PASS
  }
  else if(b_length <= 15)
  {
    // can print everything at once, because it's just one line
    this->_liquidCrystal.print(this->_buffer);    // print everything
  }
  else
  {
    // it's two lines of text, split it and print it
    String first_line = this->_buffer.substring(0, 15);
    String second_line = this->_buffer.substring(16, b_length);
    this->_liquidCrystal.print(first_line);
    this->_liquidCrystal.setCursor(0, 1);    // move to beginning of display
    this->_liquidCrystal.print(second_line);
  }
  this->setCursor();      // set cursor to right value 
}
// sets the cursors position on the display
// using the line and column indexes
void LCD::setCursorPosition(int line, int column)
{
  this->_current_line = line;
  this->_current_column = column;
  this->setCursor();    // enable changes
}
// gets the current line #
int LCD::getCurrentLine()
{
  return this->_current_line;
}
// gets the current column #
int LCD::getCurrentColumn()
{
  return this->_current_column;
}
// prints string to display
void LCD::printString(String s)
{
  this->_liquidCrystal.print(s);
}
// sets a new buffer
void LCD::setBuffer(String new_buffer)
{
  this->_buffer = new_buffer;
}
