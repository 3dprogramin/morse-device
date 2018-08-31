// icebox/
// getyourbots.com

#ifndef MorseButton_h

#define MorseButton_h

#include "Arduino.h"
#include "Config.h"

class MorseButton{
	public:
		void init();			// initialize button (capacitive sensor)
		boolean isPressed();	// check if button is pressed
};

#endif
