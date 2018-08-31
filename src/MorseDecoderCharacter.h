// icebox
// getyourbots.com

#ifndef MorseDecoderCharacter_h
#define MorseDecoderCharacter_h

#include "Arduino.h"

class MorseDecoderCharacter
{
  public:
    MorseDecoderCharacter();        // constructor
    void addDot();           // add dot to buffer
    void addDash();          // add dash to buffer
    char generate();         // generate character from buffer
    void reset();            // reset buffer
    boolean maxSize();       // check if buffer is max size (has 5 dots/dashes)
    boolean bufferNotEmpty();// check if buffer is empty or not
  private:
    String _buffer;          // buffer string variable

};

#endif
