#ifndef INCLUDES_H
#define INCLUDES_H

#define MIN_PWM 0
#define MAX_PWM 255

#define TOTAL_NUM_JOINTS 5

  // Arduino pin assignments
#define JOINT_0_PIN 3
#define JOINT_1_PIN 5
#define JOINT_2_PIN 6
#define JOINT_3_PIN 9
#define JOINT_4_PIN 10

// Us this flag for development
//#define IS_ARDUINO_BOARD

// Tools
//#include <Print.h>
#include "Arduino.h"


////////////////////////
// WE NEED analoWrite()
//#include <wiring_analog.h>
////////////////////////


#endif // INCLUDES_H

