#ifndef DATAMONSTER_CPP
#define DATAMONSTER_CPP

#include "Includes.h"
#include "Servo.cpp"


class DataMonster {

  ///////////////////////////
  // Private Members
  ///////////////////////////

private:
  int pinNum[];
  bool m_bIsCalib;
  String m_sFileNameFullPath;

  ///////////////////////////
  // Private Methods
  ///////////////////////////
  float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

public:
  ///////////////////////////
  // Public Members
  ///////////////////////////
  Servo* m_apJoinArray[5];

  ///////////////////////////
  // Public methods / Interface
  ///////////////////////////
  DataMonster() {  

    // Create joints for the robot
    m_apJoinArray[0] = new Servo(JOINT_0_PIN);
    m_apJoinArray[1] = new Servo(JOINT_1_PIN);
    m_apJoinArray[2] = new Servo(JOINT_2_PIN);
    m_apJoinArray[3] = new Servo(JOINT_3_PIN);
    m_apJoinArray[4] = new Servo(JOINT_4_PIN);

    // Check robot calibration / WARNING: The calibration routine is still under develpment
    m_bIsCalib = false;

#ifdef ROBOT_1
    m_apJoinArray[0]->setPwmLimits(10, 33);
    m_apJoinArray[1]->setPwmLimits(20, 31);
    m_apJoinArray[2]->setPwmLimits(8, 27);
    m_apJoinArray[3]->setPwmLimits(11, 37);
    m_apJoinArray[4]->setPwmLimits(10, 35);
#endif

#ifdef ROBOT_2
    m_apJoinArray[0]->setPwmLimits(8, 21);
    m_apJoinArray[1]->setPwmLimits(10, 23);
    m_apJoinArray[2]->setPwmLimits(8, 23);
    m_apJoinArray[3]->setPwmLimits(13, 40);
    m_apJoinArray[4]->setPwmLimits(11, 40);
#endif

  }

  // Parm: These are 3D coordinates of an object detected in space
  // The robot moves will move as a function of these values.
  // _fX [ -0.707, 0.707 ]
  // _fY [ 0, 0.707 ]
  // _fZ [ 0, 0.707 ]
  void setPosture(float _j0, float _j1, float _j2, float _j3, float _j4, bool _bGotFood)
  {
    int iPwmValue = 0;
    int iJoint = 0;

    // Move head really because the robot is happy
    if(_bGotFood)
      happyRoutine(3);

    // Set posture
    setPosture(_j0, _j1, _j2, _j3, _j4);
  }

  void setPosture(float _j0, float _j1, float _j2, float _j3, float _j4)
  {
    moveJoint(0,_j0); 
    moveJoint(1,_j1);  
    moveJoint(2,_j2);  
    moveJoint(3,_j3);  
    moveJoint(4,_j4);   
  }

  void happyRoutine(int _iJoint)
  {
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMin); 
    moveJoint(0,m_apJoinArray[0]->m_fPwmMax);   
    delay(500);
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMax); 
    moveJoint(0,m_apJoinArray[0]->m_fPwmMin);   
    delay(500);
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMin);
    moveJoint(0,m_apJoinArray[0]->m_fPwmMax);    
    delay(500);
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMax);    
    delay(500);
  }

  // Check if all the joint servos are calibrted
  boolean isCalibrated()
  {
    for (int i = 0; i < TOTAL_NUM_JOINTS; i++)
    {
      if ( !m_apJoinArray[i]->isCalibrated() )
      {
        return false;
      }
    }

    return true;
  }

  // Sets the motion limits for a particular joint
  // _bLowOrHigh = True -> Low Limit
  // _bLowOrHigh = False -> High Limit
  void setJointMotionLimit(boolean _bLowOrHigh, int _iJointNum, int _iPwmLimValue) {

    if ( 0 <= _iJointNum && _iJointNum <TOTAL_NUM_JOINTS ) {
      if ( _bLowOrHigh == true ) { // Set Low Limit
        m_apJoinArray[_iJointNum]->setPwmLowLimit(_iPwmLimValue);
      }
      else { // Set High Limit
        m_apJoinArray[_iJointNum]->setPwmHighLimit(_iPwmLimValue);
      }
    }
    else {
      Serial.println("ERROR: joint #: doesn't exist\n");
    }
  }

  void moveJointToCalib(int _iJointNum, int _sSteps) {
    if ( !m_bIsCalib )
      m_apJoinArray[_iJointNum]->setCalibSteps(_sSteps);
    else
      Serial.println("WARNING: Robot joints are calibrated. Please use moveJoint() instead.\n");
  }

  boolean moveJoint(int _iJointNum, int _sSteps) {
    if ( 0 <= _iJointNum && _iJointNum <TOTAL_NUM_JOINTS ) {
      // Try to move the joint
      if ( m_apJoinArray[_iJointNum]->setSteps(_sSteps) ) {
        // "ERROR: joint PWM out of bounds.
        return true;
      }
    }
    else {
      Serial.println("ERROR: joint #: doesn't exist\n");
      return true;
    }
    return false;
  }
};

#endif // DATAMONSTER_CPP








