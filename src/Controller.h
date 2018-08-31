// icebox
// getyourbots.com

#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "Beep.h"
#include "MorseDecoder.h"
#include "MorseEncoder.h"
#include "MorseTrainer.h"
#include "LCD.h"
#include "Config.h"

class Controller
{
  public:
    void init();                  // initializes the program
    void looper();                // loops
  private:
    void initPins();              // initialize arduino PINs
    void initMorseDecoder();       // init morse button/capacitive sensor (through morse decoder)
    void _morseSignalDecoder();          // process morse signal, and translate it to text
    void _morseSignalReplay();          // process text from display, and translate it to morse code - replay text from display
    void _removeLastCharFromDisplay();        // clears the last character from display (in case you missed a letter/digit\dot/dash)
    void _potentiometerValueChange();    // check if the potentiometer value is the same
    void _morseSignalTrainer();                // checks for morseTrainer mode, and acts
    boolean checkRemoveLastChar();      // check if last character is OK for being removed
    int _potentiometerValue = -11;       // -11 so that it's gonna change with the initialization, whatever value the potentiometer has
    LCD _lcd;                     // LCD object
    MorseDecoder _morseDecoder;                 // morse decoder object (that's what is doing that morse signal reading/processing)
    MorseEncoder _morseEncoder;                 // morse encoder object
    MorseTrainer _morseTrainer;                 // morse trainer obj
    boolean _space_ok = false;    // aux variable to help with space
    String _word = "";            // word variable, keeps track of words
    int _last_update_time;        // variable keeps track of last time when character was appended to display
    boolean _removeLastCharBtnState = false;
    boolean _replayingMorseBtnState = false;
};
#endif
