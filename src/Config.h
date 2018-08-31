// icebox
// getyourbots.com

#ifndef Config_h

// POWER CONSUMTION
// ---------------------------------------------------
// (without low power)
// CONSUMTION - 125mA
// -----------------------------------------------------
// LCD BACKLIGHT - 13mA
// ----------------
// TOTAL - 138mA

// -------------------------------------------------------------------------------------------------------
// Configuration (PINS, MorseValues, LCD, etc)
// =======================================================================================================
// Morse button PINs, other buttons, Buzzer, Led PINS
// -------------------------------------------------------------------------------------------------------
//  * pushbutton attached to BUTTON_PIN from +5V
//  * 10K resistor attached to BUTTON_PIN from ground
#define MORSE_BUTTON_PIN_SEND 3    // morse send button
#define MORSE_BUTTON_PIN_RECV 4    // morse recv button
// 5 here
#define REPLAY_BUTTON_PIN 7  // replay morse code from display button
#define CLEAR_BUTTON_PIN 6   // clear last character from display button
// 7 here
#define MORSE_TRAINER_BUTTON_PIN 5  // morse trainer button pin (will help in learning the letters/digits)
#define POTENTIOMETER_PIN 14    // potentiometer ANALOG pin

#define LED_PIN 8		// led pin
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// Morse values and buzzer value
// -------------------------------------------------------------------------------------------------------

#define BEEP_VALUE 200		// can be from (1, 255),  0 - no beep
#define BUZZER_PIN 9	 	// PWM pin

#define MORSE_BUTTON_THRESHOLD_MIN 100    // button is pressed when capacitive sensor results exceds value

// DEPRECATED
// SET NOW USING POTENTIOMETER 
// AND USING THE MORSE CODE RULES
// ------------------------------
// morse values
/*
#define DOT_TIME_PULSE 200				// dot pulse
// IMCR - international morse code rule #4
#define SPACE_BETWEEN_LETTERS DOT_TIME_PULSE * 3	  // by rule should be * 3
// word pulse (dot * 7 is the rule, but will be 10 this time, 'typing simplicity'
#define SPACE_BETWEEN_WORDS DOT_TIME_PULSE * 7           // by rule should be * 7
*/

// others
#define POTENTIOMETER_PULSE_MIN_DIFF 10
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// LCD PINS
// -------------------------------------------------------------------------------------------------------
// SDA - A4 (pro mini)
// SCL - A5 (pro mini)
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// OTHER
// -------------------------------------------------------------------------------------------------------
#define SERIAL_DEBUG true    // will print debug info to serial (9600 baud)

// maximum buffer for encoder array (can't go more than 200 based for our display)
#define MAX_ENCODER_BUFFER 200    
#define LCD_PULSE_VIEW_BEFORE_SWITCH 600    // keep LCD pulse view mode for this amount of millis
// =======================================================================================================

#define Config_h
#include "Arduino.h"
#endif
