//#include "Servo.cpp"g_oDataMonster
#include "DataMonster.cpp"

// Function Signatures
void getSerialCommand();


// Globals
//Servo* g_poServo;
DataMonster* g_poDataMonster;

int g_iPwmValue = 128;
bool g_bSettingLowLimit = true;
//float g_fAngle = 0;
//float g_fOneDegInRad = 0;
int g_iJointSelect;
int g_iJointCounter;
long g_iJointUpdateTimerCounter;
int JOINT_UPDATE_TIMER_COUNTER_LIMIT = 10;

int g_iByte = 0;

#define UP 119 // w
#define DOWN 115 // s
#define LEFT 97 // a
#define RIGHT 100 // d
#define SPACE 32 // space 

// Test
int g_iLed13 = 13;

void setup() {

  // Blinking Led
  pinMode(g_iLed13, OUTPUT);     

  // Setup Servos
  //  g_poServo = new Servo(3);
  //  g_poServo->setPwmLimits(0, 255);

  g_poDataMonster = new DataMonster();

  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // prints title with ending line break 
  Serial.println("ASCII Table ~ Character Map"); 

}

int iCount = 0;
bool bIncrementing = true;
void loop() {

  /////////////////////////////////
  // Run Main Code
  /////////////////////////////////
  // IF g_oSensorModule.foundPerson()
  // [X,Y,Z] = g_oSensorModule.getPersonsLocation()
  // g_poDataMonster->setMood(FOCUSED)
  // ELSE
  // [X,Y,Z] = g_oSensorModule.getNextDataPosture()
  // g_poDataMonster->setMood(CHILLING)

  // g_poDataMonster->setPosture([X,Y,Z])

  // Check Serial for command
  getSerialCommand();

  //////////////
  // Check if Robot is calibrates  
  /////////////
 // if ( !g_poDataMonster->isCalibrated() )
//  { 
    // Calibrate Robot
  //  calibRobot();
//  }
 // else
//  {
    // Control Robot
    controlRobot();
//  }



  //
  //  g_poServo->setSteps(iCount);
  //  //delay(1000);               // wait for a second
  //  Serial.println(iCount);
  //
  //  if(bIncrementing)
  //  {
  //    iCount++;
  //    if(iCount==256)
  //    {
  //      iCount = 255;
  //      bIncrementing = false; 
  //    }
  //  }
  //  else
  //  {
  //    iCount--;
  //    if(iCount==-1)
  //    {
  //      iCount = 0;
  //      bIncrementing = true; 
  //    }
  //  }


  // Indicate we are hitting the Loop() function
  // blinkPin13();


}

// Robot calibration interface
void calibRobot()
{
  //print("Calibrating Robot\n");

  // Update the joint position every 100 draw cycles
  g_iJointUpdateTimerCounter++;
  if ( (g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT) == 0) {

    // Reset background
    //background(0);

    if (g_bSettingLowLimit)  
    {
      Serial.print("Calibrating | Joint: ");
      Serial.print(g_iJointSelect);
      Serial.print(" | Limit: LOW | PWM Value: ");
      Serial.println(g_iPwmValue);
    }
    else
    {
      Serial.print("Calibrating | Joint: ");
      Serial.print(g_iJointSelect);
      Serial.print(" | Limit: HIGH | PWM Value: ");
      Serial.println(g_iPwmValue);
    }

    // Move the joint to the next PWM value
    //g_poDataMonster->moveJoint(g_iJointSelect, g_iPwmValue);
    g_poDataMonster->moveJointToCalib(g_iJointSelect, g_iPwmValue);

    //print("Joint: " + g_iJointSelect + " Command: " + g_fAngle + " \n");

    // print("Count: " + g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT + "\n");
    //servoVis.display passes the servo input variable for that servo.
    //myServoVis.display(g_iPwmValue);
  }
}

// Robot calibration interface
void carlosControlRobot()
{
  //print("Calibrating Robot\n");

  // Update the joint position every 100 draw cycles
  g_iJointUpdateTimerCounter++;
  if ( (g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT) == 0) {

    // Reset background
    //  background(0);

    // Move the joint to the next PWM value
    g_poDataMonster->moveJoint(g_iJointSelect, g_iPwmValue);
    //g_poDataMonster->moveJointToCalib(g_iJointSelect, g_iPwmValue);

    //print("Joint: " + g_iJointSelect + " Command: " + g_fAngle + " \n");

    // print("Count: " + g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT + "\n");
    //servoVis.display passes the servo input variable for that servo.
    //    myServoVis.display(g_iPwmValue);
  }
}


// Robot control
void controlRobot()
{
  // print("Controlling Robot\n");

  // Control Robot Code
  carlosControlRobot(); // This replaces all the steps below. Just testing.

  // 1) Input: Get data stream / Get camera stream

  // 2) Process: Get data stream / process camera

  // 3) Ouput: Actuate Robot

    // 4) Update GUI
}

void getSerialCommand() {
  if (Serial.available() > 0) {
    // get incoming byte:
    g_iByte = Serial.read();
    //Serial.println(g_iByte);

    if (g_iByte == UP) {
      g_iPwmValue += 10;
      //g_fAngle += g_fOneDegInRad;
    } 
    else if (g_iByte == DOWN) {
      g_iPwmValue -= 10;
      //g_fAngle -= g_fOneDegInRad;
    }
    else if (g_iByte == LEFT) {

      g_iPwmValue = 128;
      //g_fAngle = 0;
      g_iJointCounter--;
    } 
    else if (g_iByte == RIGHT) {

      g_iPwmValue = 128;
      //g_fAngle = 0;
      g_iJointCounter++;
    }
 //   else if (g_iByte == SPACE) {
      //print("ENTER........\n");

//      // Set values and calibrate servo/joint
//      if ( !g_poDataMonster->isCalibrated() )
//      {
//        if ( g_bSettingLowLimit )
//        { // Set Low Limit
//          //Serial.print("Joint's " + g_iJointSelect + " LOW limit has been set to: " + g_iPwmValue + "\n");  
//          g_bSettingLowLimit = !g_bSettingLowLimit;
//        }
//        else
//        { // Set High Limit
//          //print("Joint's " + g_iJointSelect + " HIGH limit has been set to: " + g_iPwmValue + "\n");
//          g_iJointCounter++;  
//          g_bSettingLowLimit = !g_bSettingLowLimit;
//        }
//
//        if (g_iPwmValue < 0)
//          g_iPwmValue = 0;
//        if (g_iPwmValue > 255)
//          g_iPwmValue = 255;
//
//        if (g_iJointCounter < 0)
//          g_iJointCounter = 0;
//        if (g_iJointCounter > TOTAL_NUM_JOINTS-1)
//          g_iJointCounter = TOTAL_NUM_JOINTS-1;
//
//        g_poDataMonster->setJointMotionLimit(g_bSettingLowLimit, g_iJointCounter, g_iPwmValue);

        //        // Print value to the calibration file
        //        g_oCalibFileWriter.print(Integer.toString(g_iPwmValue) + TAB);
        //
        //        // Close the file when done calibrating
        //        if ( g_iJointCounter == TOTAL_NUM_JOINTS )
        //        {
        //          g_oCalibFileWriter.flush();  // Writes the remaining data to the file
        //          g_oCalibFileWriter.close();  // Finishes the fil
        //        }
      //}
  //  }
  }

        if (g_iPwmValue < 0)
          g_iPwmValue = 0;
        if (g_iPwmValue > 255)
          g_iPwmValue = 255;

        if (g_iJointCounter < 0)
          g_iJointCounter = 0;
        if (g_iJointCounter > TOTAL_NUM_JOINTS-1)
          g_iJointCounter = TOTAL_NUM_JOINTS-1;

  /*
  if (g_fAngle < -HALF_PI)
   g_fAngle = HALF_PI;
   if (g_fAngle > HALF_PI)
   g_fAngle = HALF_PI;
   */

  g_iJointSelect = g_iJointCounter;
  //  g_iJointSelect = g_iJointCounter % TOTAL_NUM_JOINTS;
  // print("Joint: " + g_iJointSelect + "\n");

}

//////////
void blinkPin13()
{
  digitalWrite(g_iLed13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(g_iLed13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}




