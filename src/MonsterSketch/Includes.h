#ifndef INCLUDES_H
#define INCLUDES_H

// ***************************************
// Specify which Robot we are programming
// ***************************************

//#define ROBOT_1 // Glued sensors
#define ROBOT_2 // Screwed sensors

// ***************************************
// Specify which Robot we are programming
// ***************************************
//#define CALIBRATING

//////////////////////////////////////////
// Network Settings
//////////////////////////////////////////
//#define NETWORK_SSID "BlackSwan_Portal"
//#define NETWORK_PASS "88288818"
#define NETWORK_SSID "makerfaire"
#define NETWORK_PASS ""


#define TWITTER_MODULE_STATUS_LED_PIN 7

#define JSON_BUFFER_SIZE 512

//#define WIFI

//////////////////////////////////////////
// Robot Macros
//////////////////////////////////////////
#define MIN_PWM 8
#define MAX_PWM 40

#define TOTAL_NUM_JOINTS 5

// Arduino pin assignments

#define JOINT_0_PIN 11
#define JOINT_1_PIN 5
#define JOINT_2_PIN 6
#define JOINT_3_PIN 9
#define JOINT_4_PIN 10

//////////////////////////////////////////
// Sensor Macros
//////////////////////////////////////////
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

//////////////////////////////////////////
// Project Incluse
//////////////////////////////////////////
#include "Arduino.h"

#endif // INCLUDES_H


