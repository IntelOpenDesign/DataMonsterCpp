#ifndef SENSOR_MODULE_CPP
#define SENSOR_MODULE_CPP

//#include "Arduino.h"
#include "Includes.h"

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
  SensorModule(){
    ///  will need to move these to the update loop once they're inputing values dynamically
    /////////////////////////////springValue (usually small- (0.001-.1)
    springValue= 0.01;
    /////////////////////////////////////////////////////////////////////

    ////////////////////////////dampening value (1= no dampening, 0 = full dampening (no movement)) Usually .9 or so.
    dampening = 0.9;
    //////////////////////////////////////////////////////////////////////

for (int i=0; i<3; i++){
      location[i] = 0;
}
for (int i=0; i<3; i++){
      force[i] = 0;
}
for (int i=0; i<3; i++){
      magnitude[i] = 0;
}


    //feed in moods, etc
  }

  void update(){
    sensorL = analogRead(A0);
    //delay(100);
    sensorR = analogRead(A1);
    //delay(100);
    sensorUP = analogRead(A2);
    //delay(100);
    left = map( sensorL, 0, 1023, 0, -1000);
    right = map( sensorR, 0, 1023, 0, 1000);
    up = map( sensorUP, 0, 1023, 0, 1000);

    //set x, y, and z values for lxyz
    lxyz[0] = (sin(PI/4)*(left/1000)); 
    lxyz[1] = (0);
    lxyz[2] = (cos(PI/4)*(left/1000));

    //set x, y, and z values for rxyz
    rxyz[0] = (sin(PI/4)*(right/1000)); 
    rxyz[1] = (0);
    rxyz[2] = (cos(PI/4)*(right/1000));

    //set x, y, and z values for upxyz
    rxyz[0] = (0); 
    rxyz[1] = (sin(PI/4)*(up/1000));
    rxyz[2] = (cos(PI/4)*(up/1000));

    for (int i=0; i<3; i++){
      target[i] =  (lxyz[i] + rxyz[i] + upxyz[i]);
    }
    /////////////////////////////////////////////////////////Spring and Dampening code///////////////////////////////////
    for (int i=0; i<3; i++){
      magnitude[i] = target[i]-location[i];
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


