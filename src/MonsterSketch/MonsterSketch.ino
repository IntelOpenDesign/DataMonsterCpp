#include "Servo.cpp"

// Globals
Servo* g_poServo;

// Test
int g_iLed13 = 13;

void setup() {

  // Blinking Led
  pinMode(g_iLed13, OUTPUT);     

  // Setup Servos
  g_poServo = new Servo(3);
  g_poServo->setPwmLimits(65, 160);

}

int iCount = 0;
bool bIncrementing = true;
void loop() {

  /////////////////////////////////
  // Run Main Code
  /////////////////////////////////
  // IF g_oSensorModule.foundPerson()
    // [X,Y,Z] = g_oSensorModule.getPersonsLocation()
    // g_oDataMonster.setMood(FOCUSED)
  // ELSE
    // [X,Y,Z] = g_oSensorModule.getNextDataPosture()
    // g_oDataMonster.setMood(CHILLING)

  // g_oDataMonster.setPosture([X,Y,Z])

  g_poServo->setSteps(iCount);
  delay(1000);               // wait for a second

  if(bIncrementing)
  {
    iCount++;
    if(iCount==256)
    {
      iCount = 255;
      bIncrementing = false; 
    }
  }
  else
  {
    iCount--;
    if(iCount==-1)
    {
      iCount = 0;
      bIncrementing = true; 
    }
  }


  // Indicate we are hitting the Loop() function
  blinkPin13();


}

//////////
  void blinkPin13()
  {
    digitalWrite(g_iLed13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(g_iLed13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);               // wait for a second
  }


