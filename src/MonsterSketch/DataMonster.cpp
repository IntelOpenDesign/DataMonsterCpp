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
  Servo* m_apJoinArray[5];
  bool m_bIsCalib;

  //private static final String m_sCalibFileName = "JointCalibration.txt";
  String m_sFileNameFullPath;

  //private BufferedReader m_oCalibFile;

  ///////////////////////////
  // Private Methods
  ///////////////////////////



  ///////////////////////////
  // Public Members
  ///////////////////////////
public:


  ///////////////////////////
  // Public methods / Interface
  ///////////////////////////
  DataMonster() {  
    // Setting up interface with Ardtruino board 
    Serial.println("Print List:");

//    m_apJoinArray = new Servo[TOTAL_NUM_JOINTS] ;

    // Create joints for the robot
    m_apJoinArray[0] = new Servo(JOINT_0_PIN);
    m_apJoinArray[1] = new Servo(JOINT_1_PIN);
    m_apJoinArray[2] = new Servo(JOINT_2_PIN);
    m_apJoinArray[3] = new Servo(JOINT_3_PIN);
    m_apJoinArray[4] = new Servo(JOINT_4_PIN);

    // Check robot calibration / WARNING: The calibration routine is still under develpment
    m_bIsCalib = false;

    // Set calibration names
    //m_sFileNameFullPath = dataPath(m_sFileName);
    //m_oCalibFile = createReader(m_sFileName);   

    // Calibrate the robot joints  
//    m_apJoinArray[0]->setPwmLimits(65, 160);
//     m_apJoinArray[1]->setPwmLimits(113, 175);
//     m_apJoinArray[2]->setPwmLimits(113, 175);
//     m_apJoinArray[3]->setPwmLimits(55, 150);
//     m_apJoinArray[4]->setPwmLimits(55, 175);
     
     m_apJoinArray[0]->setPwmLimits(55, 245);
     m_apJoinArray[1]->setPwmLimits(0, 255);
     m_apJoinArray[2]->setPwmLimits(10, 255);
     m_apJoinArray[3]->setPwmLimits(55, 240);
     m_apJoinArray[4]->setPwmLimits(60, 228);
     
  }

  // Check if all the joint servos are calibrted
  boolean isCalibrated()
  {
    for (int i = 0; i < TOTAL_NUM_JOINTS; i++)
    {
      if ( !m_apJoinArray[i]->isCalibrated() )
      {
        //Serial.println("WARNING: joint #" + i + " is not calibrated.\n");
        //m_bIsCalib = false; 
        //return m_bIsCalib;
        return false;
      }
    }

    //m_bIsCalib = true; 
    //return m_bIsCalib;
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
      //Serial.println(String"ERROR: joint #: " + _iJointNum + " doesn't exist\n");
      Serial.println("ERROR: joint #: doesn't exist\n");
    }
  }

  void moveJointToCalib(int _iJointNum, int _sSteps) {
    if ( !m_bIsCalib )
      m_apJoinArray[_iJointNum]->setCalibSteps(_sSteps);
    else
      Serial.println("WARNING: Robot joints are calibrated. Please use moveJoint() instead.\n");
  }

  // Send a command to the servo on the given joint
  /*
  boolean moveJoint(int _iJointNum, float _fAngle) {
    if ( 0 <= _iJointNum && _iJointNum <TOTAL_NUM_JOINTS ) {
      // Try to move the joint
      if ( m_apJoinArray[_iJointNum]->setAngle(_fAngle) ) {
        Serial.println("ERROR: joint angle out of bounds.\n");
        return true;
      }
    }
    else {
      Serial.println("ERROR: joint #: " + _iJointNum + " doesn't exist\n");
      return true;
    }
    return false;
  }
  */

  boolean moveJoint(int _iJointNum, int _sSteps) {
    if ( 0 <= _iJointNum && _iJointNum <TOTAL_NUM_JOINTS ) {
      // Try to move the joint
      if ( m_apJoinArray[_iJointNum]->setSteps(_sSteps) ) {
        Serial.println("ERROR: joint PWM out of bounds.\n");
        return true;
      }
    }
    else {
      //Serial.println("ERROR: joint #: " + _iJointNum + " doesn't exist\n");
      Serial.println("ERROR: joint #: doesn't exist\n");
      return true;
    }
    return false;
  }
};




#endif // DATAMONSTER_CPP


