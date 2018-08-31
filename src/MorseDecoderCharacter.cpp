// icebox
// getyourbots.com

#include "Arduino.h"
#include "MorseDecoderCharacter.h"

MorseDecoderCharacter::MorseDecoderCharacter()
{
  this->reset();    // make buffer = ""
}
// add dot to buffer
void MorseDecoderCharacter::addDot()
{
  this->_buffer += ".";
}
// add dash to buffer
void MorseDecoderCharacter::addDash()
{
  this->_buffer += "_";
}
// generate character from buffer
char MorseDecoderCharacter::generate()
{
  String b = this->_buffer;    // use a local variable (easier to write the code)
  if(b.equals("._"))           // 'A'
  {
     return 'A';
  }
  else if(b.equals("_..."))    // 'B'
  {
     return 'B';
  }
  else if(b.equals("_._."))    // 'C'
  {
     return 'C';
  }
  else if(b.equals("_.."))    // 'D'
  {
     return 'D';
  }
  else if(b.equals("."))      // 'E' 
  {
     return 'E';
  }
  else if(b.equals(".._."))   // 'F'
  {
     return 'F';
  }
  else if(b.equals("__."))    // 'G'
  {
     return 'G';
  }
  else if(b.equals("...."))   // 'H'
  {
     return 'H';
  }
  else if(b.equals(".."))    // 'I'
  {
     return 'I';
  }
  else if(b.equals(".___"))  // 'J'
  {
     return 'J';
  }
  else if(b.equals("_._"))   // 'K'
  {
     return 'K';
  }
  else if(b.equals("._.."))  // 'L'
  {
     return 'L';
  }
  else if(b.equals("__"))    // 'M'
  {
     return 'M';
  }
  else if(b.equals("_."))    // 'N'
  {
     return 'N';
  }
  else if(b.equals("___"))   // 'O'
  {
     return 'O';
  }
  else if(b.equals(".__."))  // 'P'
  {
     return 'P';
  }
  else if(b.equals("__._"))  // 'Q'
  {
     return 'Q';
  }
  else if(b.equals("._."))   // 'R'
  {
     return 'R';
  }
  else if(b.equals("..."))   // 'S'
  {
     return 'S';
  }
  else if(b.equals("_"))     // 'T'
  {
     return 'T';
  }
  else if(b.equals(".._"))   // 'U'
  {
     return 'U';
  }
  else if(b.equals("..._"))  // 'V'
  {
     return 'V';
  }
  else if(b.equals(".__"))   // 'W'
  {
     return 'W';
  }
  else if(b.equals("_.._"))  // 'X'
  {
     return 'X';
  }
  else if(b.equals("_.__"))  // 'Y'
  {
     return 'Y';
  }
  else if(b.equals("__.."))  // 'Z'
  {
     return 'Z';
  }
  

  else if(b.equals(".____"))  // '1'
  {
     return '1';
  }
  else if(b.equals("..___"))  // '2'
  {
     return '2';
  }
  else if(b.equals("...__"))  // '3'
  {
     return '3';
  }
  else if(b.equals("...._"))  // '4'
  {
     return '4';
  }
  else if(b.equals("....."))  // '5'
  {
     return '5';
  }
  else if(b.equals("_...."))  // '6'
  {
     return '6';
  }
  else if(b.equals("__..."))  // '7'
  {
     return '7';
  }
  else if(b.equals("___.."))  // '8'
  {
     return '8';
  }
  else if(b.equals("____."))  // '9'
  {
    return '9';
  }
  else if(b.equals("_____"))  // '0'
  {
     return '0';
  }
  return '$';
}
// check if it's under size (under 5 . or _, 5 being the maximum)
boolean MorseDecoderCharacter::maxSize()
{
  int s = this->_buffer.length();
  return s == 5;
}
// reset the buffer
void MorseDecoderCharacter::reset()
{
  this->_buffer = "";
}
boolean MorseDecoderCharacter::bufferNotEmpty()
{
  return this->_buffer.length() > 0;
}
