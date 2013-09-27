#ifndef SERVO_CPP
#define SERVO_CPP

#include "Includes.h"
#include "Wire.h"

class Servo {

  ///////////////////////////
  // Private Members
  ///////////////////////////
private:
  int m_iPinNum ;

  //  static const int MIN_PWM = 0;
  //  static const int MAX_PWM = 255;
  //  static const int MIN_ANGLE = 0;
  //  static const float MAX_ANGLE = PI;
  //float PWM_TO_RAD = 81.4873; // WARNING: This is an approximation assuming the mapping [0 - 255] <-> [-PI/2 - PI/2] 


  float m_fPwmMidPoint;

  float m_fAngMax;
  float m_fAngMin;

  bool m_bIsServoCalib;

  ///////////////////////////
  // Private Methods
  ///////////////////////////

  // Set the PWM percent
  // [0 - 255] -> [0% - 100%] 
  void setPwm(int _iPwmVal) { 
    if ( MIN_PWM <= _iPwmVal && _iPwmVal <= MAX_PWM) {
      setPwmI2C(m_iPinNum, _iPwmVal);
    }
    else {
      setPwmI2C(m_iPinNum, (int)(MAX_PWM/2));
    }
  }

  void setPwmI2C(int _iPinNum, int _iPwmVal)
  {
    // Select pin to write I2C commands to
     analogWrite(_iPinNum,1);
    
    // Set divider to get 125Hz freq.
    Wire.beginTransmission(0x20);
    Wire.write(0x2C);
    Wire.write(0x06);
    Wire.endTransmission();

    // Select programmable PWM CLK source
    Wire.beginTransmission(0x20);
    Wire.write(0x29);
    Wire.write(0x04);
    Wire.endTransmission();

    // Set period register
    Wire.beginTransmission(0x20);
    Wire.write(0x2a);
    Wire.write(0xff);
    Wire.endTransmission();

    // Set minimum duty cycle (31us @ 125Hz)
    Wire.beginTransmission(0x20);
    Wire.write(0x2b);
    Wire.write(_iPwmVal);
    Wire.endTransmission();
  }

  ///////////////////////////
  // Public Members
  ///////////////////////////
public:
  float m_fPwmMin;
  float m_fPwmMax;

  ///////////////////////////
  // Public methods / Interface
  ///////////////////////////


  // Constructor
  Servo(int _iPinNum) {

    m_iPinNum = _iPinNum;
    m_fPwmMidPoint = 128;

    // Setting up important values
    //   PWM_TO_RAD = MAX_PWM / MAX_ANGLE;
    m_fPwmMin = -1;
    m_fPwmMax = -1;
    //    m_fAngMax = -1;
    //   m_fAngMin = -1;

    // Enable I2C to control servo
    Wire.begin();

    // Assign servo pin
    setPin(_iPinNum);



    m_bIsServoCalib = false;
  }

  // Sets the physical pin number for
  // the Servo's PWM signal 
  void setPin(int _iPinNum) {
    m_iPinNum = _iPinNum;
    pinMode(m_iPinNum, OUTPUT);
  }

  // Returns the physical pin number
  // to which the servo is attached in
  // Arduino board
  int getPin() {
    return m_iPinNum;
  }

  // Arduino board
  void setPwmLimits(int _iMin, int _iMax) {
    setPwmLowLimit(_iMin);
    setPwmHighLimit(_iMax);
  }

  void setPwmLowLimit(int _iMin) {
    m_fPwmMin = (float)(_iMin);
    calibPwmLimits();
  }

  void setPwmHighLimit(int _iMax) {
    m_fPwmMax = (float)(_iMax);
    calibPwmLimits();
  }

  bool isCalibrated()
  {
    return m_bIsServoCalib;
  }

  void calibPwmLimits() {
    if (m_fPwmMin >0 && m_fPwmMax >0)
    {
      // Calculate mid point
      m_fPwmMidPoint = (m_fPwmMax-m_fPwmMin)/2.0;

      // Calculate angle offset
      // m_fAngMax = (m_fPwmMax-m_fPwmMidPoint)/PWM_TO_RAD;
      // m_fAngMin = (m_fPwmMin-m_fPwmMidPoint)/PWM_TO_RAD;

      //print("m_fAngMax: " + m_fAngMax + "\n");
      //print("m_fAngMin: " + m_fAngMin + "\n");

      m_bIsServoCalib = true;
    }
    else
    {
      Serial.println("ERROR: PWM MIN and MAX have not been set. Pleae use setPwmLowLimit() and setPwmHighLimit() first.\n");
    }
  }

  // Set angle from 0 - 180 deg // THIS MAY CHANGE IN THE FUTURE
  // For now angle and PWM are a 1:1 mapping. We'll change this in a future calibration routine
  /*
   bool setAngle(float _fAngle) {
   if ( m_fAngMin <= _fAngle && _fAngle <= m_fAngMax ) {
   int iPwnVal = (int)(_fAngle*PWM_TO_RAD);
   setPwm( iPwnVal );
   }
   else {
   return true;
   }
   
   return false;
   }    
   */

  // Once calibrated this is the function used
  bool setSteps(int _iSteps) {
    if ( m_fPwmMin <= _iSteps && _iSteps <= m_fPwmMax ) {
      setPwm( _iSteps );
    }
    else {
      return true;
    }

    return false;
  }

  // This function is used only for calibration purposes
  void setCalibSteps(int _iSteps) {
    setPwm( _iSteps );
  }

  // Invert PWM signal 

  // Calibrate

  // Smoothing function
};

#endif // SERVO_CPP









