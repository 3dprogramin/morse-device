// icebox
// getyourbots.com

#include "Controller.h"

Controller controller;  // controller object

void setup() {
  controller.init();    // initialize the controller
}

void loop() {  
  controller.looper();  // run the program looper
}
 
