#ifndef INCLUDES_H
#define INCLUDES_H

// ***************************************
// Specify which Robot we are programming
// ***************************************

#define ROBOT_1
//#define ROBOT_2

// ***************************************
// Specify which Robot we are programming
// ***************************************
#define CALIBRATING


//////////////////////////////////////////
#define MIN_PWM 0
#define MAX_PWM 255

#define TOTAL_NUM_JOINTS 5

  // Arduino pin assignments

#ifdef ROBOT_1
#define JOINT_0_PIN 11
#endif

#ifdef ROBOT_2
#define JOINT_0_PIN 3
#endif

#define JOINT_1_PIN 5
#define JOINT_2_PIN 6
#define JOINT_3_PIN 9
#define JOINT_4_PIN 10

#define SENSOR_MODULE_STATUS_LED_PIN 8
#define TWITTER_MODULE_BUTTON_PIN 2
#define TWITTER_MODULE_STATUS_LED_PIN 4

// Sensor limits
#define SENSOR_X_MIN -0.6
#define SENSOR_X_MAX 0.6
#define SENSOR_Y_MIN 0.0
#define SENSOR_Y_MAX 0.6
#define SENSOR_Z_MIN 0.0
#define SENSOR_Z_MAX 0.6

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

