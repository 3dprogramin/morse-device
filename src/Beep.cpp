// icebox
// getyourbots.com

#include "Arduino.h"
#include "Beep.h"
#include "Config.h"

// beep up
void Beep::beep_up()
{
  analogWrite(BUZZER_PIN, BEEP_VALUE);
}
// beep down
void Beep::beep_down()
{
  analogWrite(BUZZER_PIN, 0);
}

