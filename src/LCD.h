// icebox
// getyourbots.com

#ifndef LCD_h

#define LCD_h

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Config.h"

class LCD
{
  public:
    void init();                    // initialize LCD obj
    void addCharacter(char c);      // add character (to buffer, and print as well)
    void printCharacter(char c, boolean change_position = false);    // only prints a character (without adding it to buffer)
    void printString(String str);    // prints string to display
    String getBuffer();                // gets the buffer
    void setBuffer(String new_buffer);    // sets a new buffer
    void resetLcd(boolean reset_buffer = true);                // clear lcd
    void removeLastChar(boolean remove_from_buffer = true);          // remove last char from display
    boolean bufferNotEmpty();       // tells if we have something in the buffer
    void dotValueChanged(int value);// dot pulse value changed, show it, and keep track of current display
    boolean shouldSwitch();         // check if it should switch back to normal
    void switchToNormal();              // switch to normal display
    void setCursor();               // set cursor
    int getCurrentLine();           // get current line #
    int getCurrentColumn();         // gets the current column #
    void setCursorPosition(int line, int column);    // sets the cursor position using line and column (counting starts from 0)
  private:
    void initLcd();                 // init LCD, using modified LiquidCrystal library
    void resetBuffer();             // reset the buffer
    void initLcdVars();             // init/reset LCD column/line vars
    void boot();                    // boot method, prints at device start/restart
    String _buffer;                 // private buffer variable
    LiquidCrystal_I2C _liquidCrystal;   // liquid crystal object
    int _current_line;              // keeps track of cursor line
    int _current_column;            // keeps track of cursor column
    int _current_line_bak;          // backup (well, copy actually) of current line
    int _current_column_bak;        // same
    boolean _pulse_view_mode = false; // it's true when the dot pulse was changed and displayed
    int _pulse_view_timestamp = 0;  // set the pulse view timestamp
};
#endif
