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
      m_bIsServoCalib = true;
    }
    else
    {
      Serial.println("ERROR: PWM MIN and MAX have not been set. Pleae use setPwmLowLimit() and setPwmHighLimit() first.\n");
    }
  }

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
};

#endif // SERVO_CPP










