#ifndef SENSOR_MODULE_CPP
#define SENSOR_MODULE_CPP

#include "Arduino.h"

#define MAX_LOCATION 1.0 // Sensors out of bounds threshold


class SensorModule{
  //trying to do this without a vector class- only need a few addition/subtraction/mult opps.

  // target and location are accessible from the main program.  
  //Location is value after dampening.  Target is the raw sensor value mapped to x,y,z space 
public:
  float target[3];
  float location[3];

  float dampening;
  float springValue;


  //raw sensor values- left, right and up
  float sensorL;
  float sensorR;
  float sensorUP;

  //these are the individualsensor values ampped to xyz space, the left and right sensors only have x, and z. the up sensor has y, and z.
private:
  float lxyz[3];
  float rxyz[3];
  float upxyz[3];

  //sensor values mapped to 0-1
  float left;
  float right;
  float up;

  //for calculating spring effects
  float velocity[3];
  float force[3];
  float magnitude[3];


public:
  //Constructor
  SensorModule(int _iStatusLed){
    ///  will need to move these to the update loop once they're inputing values dynamically
    /////////////////////////////springValue (usually small- (0.001-.1)
    springValue = 0.01;
    /////////////////////////////////////////////////////////////////////

    ////////////////////////////dampening value (1= no dampening, 0 = full dampening (no movement)) Usually .9 or so.
    dampening = 0.9;
    //////////////////////////////////////////////////////////////////////

    for (int i=0; i<3; i++){
      location[i] = 0.1;
    }
    for (int i=0; i<3; i++){
      force[i] = 0.1;
    }
    for (int i=0; i<3; i++){
      magnitude[i] = 0.1;
    }
    //feed in moods, etc

    // Initialize status LED
    pinMode(_iStatusLed, OUTPUT);      // sets the digital pin as output
    digitalWrite(_iStatusLed,LOW);

  }
  void getLocation(float& _fX, float& _fY, float& _fZ)
  {
    _fX = 0;
    _fY = 0;
    _fZ = 0;

    if( -MAX_LOCATION <= location[0] || location[0] <= MAX_LOCATION)
      _fX = location[0];

    if( -MAX_LOCATION <= location[1] || location[1] <= MAX_LOCATION)
      _fY = location[1];     

    if( -MAX_LOCATION <= location[2] || location[2] <= MAX_LOCATION)
      _fZ = location[2];

  }

  void update(){
    sensorL = analogRead(A0);
    //delay(100);
    sensorR = analogRead(A1);
    //delay(100);
    sensorUP = analogRead(A2);
    //delay(100);


//    Serial.print(sensorL, DEC);
//    Serial.print(",");
//    delay(1);
//    Serial.print(sensorR, DEC);
//    Serial.print(",");
//    delay(1);
//    Serial.print(sensorUP, DEC);
//    Serial.println();

    ///////////////////////////////////////////Sensor Calibration Adjustments
    left = map( sensorL, 0, 700, 0, 1000);
    right = map( sensorR, 0, 700, 0, -1000);
    up = map( sensorUP, 0, 700, 0, 1000);

    //set x, y, and z values for lxyz
    lxyz[0] = (sin(PI/4)*(left/1000)); 
    lxyz[1] = (0);
    lxyz[2] = (cos(PI/4)*(left/1000));

    //set x, y, and z values for rxyz
    rxyz[0] = (sin(PI/4)*(right/1000)); 
    rxyz[1] = (0);
    rxyz[2] = (cos(PI/4)*(right/-1000));

    //set x, y, and z values for upxyz
    upxyz[0] = (0); 
    upxyz[1] = (sin(PI/4)*(up/1000));
    upxyz[2] = (cos(PI/4)*(up/1000));


    target[0] =  (lxyz[0] + rxyz[0]);
    target[1] = (upxyz[1]);

    /////  in Z space, we have three contributors, we want to take the largest value of the three.
    if (lxyz[2] > rxyz[2] && lxyz[2] > upxyz[2]){
      target[2] = lxyz[2];
    }
    else if (rxyz[2] > upxyz[2]){
      target[2] = rxyz[2];
    }
    else {
      target[2] = upxyz[2];
    }


    /////////////////////////////////////////////////////////Spring and Dampening code///////////////////////////////////
    for (int i=0; i<3; i++){
      magnitude[i] = target[i] - location[i];
    }  
    for (int i=0; i<3; i++){
      force[i] = (magnitude[i] * springValue);
    }  
    for (int i=0; i<3; i++){
      velocity[i] = dampening * (velocity[i] + force[i]);
    }  
    for (int i=0; i<3; i++){
      location[i] = location[i] + velocity[i];
    }  

  }

};

#endif // SENSOR_MODULE_CPP






