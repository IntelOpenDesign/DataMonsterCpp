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

// Sensor limits
#define SENSOR_X_MIN -0.707
#define SENSOR_X_MAX 0.707
#define SENSOR_Y_MIN 0.0
#define SENSOR_Y_MAX 0.707
#define SENSOR_Z_MIN 0.0
#define SENSOR_Z_MAX 0.707

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

