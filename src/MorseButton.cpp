// icebox
// getyourbots.com

#include "Arduino.h"
#include <CapacitiveSensor.h>
#include "MorseButton.h"
#include "Config.h"

CapacitiveSensor cap_sensor = CapacitiveSensor(MORSE_BUTTON_PIN_SEND, MORSE_BUTTON_PIN_RECV);

void MorseButton::init()
{
  cap_sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
}
boolean MorseButton::isPressed()
{
    long total =  cap_sensor.capacitiveSensor(30);
    return total >= MORSE_BUTTON_THRESHOLD_MIN;
}

