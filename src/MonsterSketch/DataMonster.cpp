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

  //private static final String m_sCalibFileName = "JointCalibration.txt";
  String m_sFileNameFullPath;

  //private BufferedReader m_oCalibFile;

  ///////////////////////////
  // Private Methods
  ///////////////////////////
  // http://forum.arduino.cc/index.php/topic,3922.0.html
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
    // Setting up interface with Ardtruino board 
    //    Serial.println("Print List:");

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

    //    m_apJoinArray[0]->setPwmLimits(55, 245);
    //    m_apJoinArray[1]->setPwmLimits(0, 255);
    //    m_apJoinArray[2]->setPwmLimits(10, 255);
    //    m_apJoinArray[3]->setPwmLimits(55, 240);
    //    m_apJoinArray[4]->setPwmLimits(60, 228);

#ifdef ROBOT_1
    m_apJoinArray[0]->setPwmLimits(55, 235);
    m_apJoinArray[1]->setPwmLimits(70, 200);
    m_apJoinArray[2]->setPwmLimits(60, 230);
    m_apJoinArray[3]->setPwmLimits(80, 200);
    m_apJoinArray[4]->setPwmLimits(70, 200);
#endif

#ifdef ROBOT_2
    m_apJoinArray[0]->setPwmLimits(50, 200);
    m_apJoinArray[1]->setPwmLimits(80, 150);
    m_apJoinArray[2]->setPwmLimits(70, 150);
    m_apJoinArray[3]->setPwmLimits(160, 150);
    m_apJoinArray[4]->setPwmLimits(70, 180);
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
    delay(500);
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMax);    
    delay(500);
    moveJoint(_iJoint,m_apJoinArray[_iJoint]->m_fPwmMin);    
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
        /////        Serial.println("ERROR: joint PWM out of bounds.\n"); // NEED
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







