//#include "Servo.cpp"g_oDataMonster
#include "DataMonster.cpp"
#include "SensorModule.cpp"
#include "TwitterModule.cpp"
#include <Ethernet.h>
#include <WiFi.h>
//#include "Includes.h"
#include "Wire.h"

// Function Signatures
void getSerialCommand();
void getPersonsLocation(float& _fX, float& _fY, float& _fZ);

// Globals
DataMonster* g_poDataMonster;
SensorModule* g_poMySensorModule;
TwitterModule* g_poTweeterListener;

//////////////////////////////////////////
// Ethernet/WiFi
//////////////////////////////////////////
int status = WL_IDLE_STATUS;     // the Wifi radio's status

bool g_bUseWiFi = false;
bool g_bGotTweet = false;

WiFiClient g_oWiFiClient;
EthernetClient client;

#define TWITTER_POLLING_TIME 100
unsigned long int g_iTwitterPollCounter = 0;
//unsigned int g_iTwitterPollCounter = 0;

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,20);
char server[] = "www.thingspeak.com"; //https://www.thingspeak.com/channels/7554/field/1/last.json
//String g_sHttpRequest = "GET /channels/7554/field/1/last.json HTTP/1.0"; // Carlos' channel
String g_sHttpRequest = "GET /channels/7556/field/1/last.json HTTP/1.0"; // Lucas' channel

unsigned long g_iLastAttemptTime = 0;
const unsigned long g_iRequestInterval = 10000;  // delay between requests
boolean g_bRequested;                   // whether you've made a request since connecting

//String currentLine = "";            // string to hold the text from server
String g_sJsonString = "";
boolean readingJsonString = false;       // if you're currently reading the tweet

//////////////////////////////////////////
int g_iPwmValue = 5;
bool g_bSettingLowLimit = true;
//float g_fAngle = 0;
//float g_fOneDegInRad = 0;
int g_iJointSelect;
int g_iJointCounter;
long g_iJointUpdateTimerCounter;
#define JOINT_UPDATE_TIMER_COUNTER_LIMIT 1000000

int g_iByte = 0;

int iCount = 0;
bool bIncrementing = true;
float g_fX = 0;
float g_fY = 0;
float g_fZ = 0;
int iPwmValue = 0;

float target[5];
float velocity[5];
float force[5];
float magnitude[5];
float location[5];

float dampening;
float springValue;

#define UP 119 // w
#define DOWN 115 // s
#define LEFT 97 // a
#define RIGHT 100 // d
#define SPACE 32 // space 

// Test
int g_iLed13 = 13;

void setup() {

  ///  will need to move these to the update loop once they're inputing values dynamically
  /////////////////////////////springValue (usually small- (0.001-.1)
  springValue = 0.03;
  /////////////////////////////////////////////////////////////////////

  ////////////////////////////dampening value (1= no dampening, 0 = full dampening (no movement)) Usually .9 or so.
  dampening = 0.9;
  //////////////////////////////////////////////////////////////////////

  for (int i=0; i<5; i++){
    target[i] = 0.1;
  }
  for (int i=0; i<5; i++){
    force[i] = 0.1;
  }
  for (int i=0; i<5; i++){
    magnitude[i] = 0.1;
  }
  for (int i=0; i<5; i++){
    location[i] = 0.1;
  }

  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Init abstract hardware classes
  g_poDataMonster = new DataMonster();
  g_poMySensorModule = new SensorModule(SENSOR_MODULE_STATUS_LED_PIN);
  g_poTweeterListener = new TwitterModule(TWITTER_MODULE_BUTTON_PIN,
  TWITTER_MODULE_STATUS_LED_PIN,
  TWITTER_MODULE_STATUS_LED_PIN);

  // Use the button to select between WiFi and Ethernet
  if( !digitalRead(TWITTER_MODULE_BUTTON_PIN) )
    g_bUseWiFi = true;

  // Init Ethernet/WiFi
  initNetwork(g_bUseWiFi);

  // prints title with ending line break 
  Serial.println("*************************************"); 
  Serial.println("***** Initializing Data Monster ****"); 
  Serial.println("*************************************"); 

}

void loop() {

  // RUN JOINT CALIBRATION ROUTINE
#ifdef CALIBRATING 

  // Check Serial for command for Robot joint calibration
  getSerialCommand();

  //////////////
  // Check if Robot is calibrated  
  /////////////
  calibRobot();

#else // RUN ROBOT PROGRAM

  /////////////////////////////////////////////
  // Get External Stimulus
  /////////////////////////////////////////////

  // Get latest stimulus from Twitter (or Button)
  String sServerString = checkTwitter(g_bUseWiFi);
  g_bGotTweet = g_poTweeterListener->gotTweet(sServerString); 

  /////////////////////////////////////////////
  // Get Object Location
  /////////////////////////////////////////////

  // Get latest data from sensor module
  g_poMySensorModule->update();
  // Get the object's location
  g_poMySensorModule->getLocation(g_fX,g_fY,g_fZ);

  /////////////////////////////////////////////
  // Set Robot
  /////////////////////////////////////////////
  //////////////////HARDCODING VALUES TO JOINTS BASED ON COMMON SENSOR VALUES BELOW////////////////////////
  // X maps to joint 0
  if(g_fX < -.1){
    iPwmValue = mapfloat(g_fX, -.1, -0.6, g_poDataMonster->m_apJoinArray[0]->m_fPwmMin, g_poDataMonster->m_apJoinArray[0]->m_fPwmMin + 50);  ///////<-hardcoded!

  } 
  else if(g_fX > 0.1){
    iPwmValue = mapfloat(g_fX, 0.1, 0.6, g_poDataMonster->m_apJoinArray[0]->m_fPwmMax , g_poDataMonster->m_apJoinArray[0]->m_fPwmMax - 50); ///////<-hardcoded!
  }
  else{
    iPwmValue = mapfloat(g_fX, SENSOR_X_MIN, SENSOR_X_MAX, g_poDataMonster->m_apJoinArray[0]->m_fPwmMin, g_poDataMonster->m_apJoinArray[0]->m_fPwmMax);
  }
  target[0]= iPwmValue;


  // Z maps to joint 1
  if(g_fZ >= 0 && g_fZ < 0.1){
    iPwmValue = mapfloat(g_fX, 0, (.1), g_poDataMonster->m_apJoinArray[1]->m_fPwmMin, g_poDataMonster->m_apJoinArray[1]->m_fPwmMax-50);  ///////<-hardcoded! 
  }
  else if(g_fZ >= 0.1 && g_fZ< 0.2){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[1]->m_fPwmMin + 50, g_poDataMonster->m_apJoinArray[1]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.2 && g_fZ< 0.3){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[1]->m_fPwmMin + 30, g_poDataMonster->m_apJoinArray[1]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.3 && g_fZ< 0.6){
    iPwmValue = mapfloat(g_fZ, 0.3, 0.6, g_poDataMonster->m_apJoinArray[1]->m_fPwmMin, g_poDataMonster->m_apJoinArray[1]->m_fPwmMax);  ///////<-hardcoded!
  }
  else{
    iPwmValue = mapfloat(g_fZ, SENSOR_Z_MIN, SENSOR_Z_MAX, g_poDataMonster->m_apJoinArray[1]->m_fPwmMin, g_poDataMonster->m_apJoinArray[1]->m_fPwmMax);
  }
  target[1]= iPwmValue;

  // Z maps to joint 2
  if(g_fZ >= 0 && g_fZ < 0.1){
    iPwmValue = mapfloat(g_fZ, 0, (.1), g_poDataMonster->m_apJoinArray[2]->m_fPwmMin, g_poDataMonster->m_apJoinArray[2]->m_fPwmMax-50);  ///////<-hardcoded! 
  }
  else if(g_fZ >= 0.1 && g_fZ< 0.2){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[2]->m_fPwmMin + 50, g_poDataMonster->m_apJoinArray[2]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.2 && g_fZ< 0.3){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[2]->m_fPwmMin + 30, g_poDataMonster->m_apJoinArray[2]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.3 && g_fZ< 0.6){
    iPwmValue = mapfloat(g_fZ, 0.3, 0.6, g_poDataMonster->m_apJoinArray[2]->m_fPwmMin, g_poDataMonster->m_apJoinArray[2]->m_fPwmMax);  ///////<-hardcoded!
  }
  else{
    iPwmValue = mapfloat(g_fZ, SENSOR_Z_MIN, SENSOR_Z_MAX, g_poDataMonster->m_apJoinArray[2]->m_fPwmMin, g_poDataMonster->m_apJoinArray[2]->m_fPwmMax);

  }
  target[2]= iPwmValue;

  // Z maps to joint 3
  if(g_fZ >= 0 && g_fZ < 0.1){
    iPwmValue = mapfloat(g_fZ, 0, (.1), g_poDataMonster->m_apJoinArray[3]->m_fPwmMin, g_poDataMonster->m_apJoinArray[3]->m_fPwmMax);  ///////<-hardcoded! 
  }
  else if(g_fZ >= 0.1 && g_fZ< 0.2){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[3]->m_fPwmMin, g_poDataMonster->m_apJoinArray[3]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.2 && g_fZ< 0.3){
    iPwmValue = mapfloat(g_fZ, 0.1, 0.2, g_poDataMonster->m_apJoinArray[3]->m_fPwmMin, g_poDataMonster->m_apJoinArray[3]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.3 && g_fZ< 0.6){
    iPwmValue = mapfloat(g_fZ, 0.3, 0.6, g_poDataMonster->m_apJoinArray[3]->m_fPwmMin, g_poDataMonster->m_apJoinArray[3]->m_fPwmMax);  ///////<-hardcoded!
  }
  else{
    iPwmValue = mapfloat(g_fZ, SENSOR_Z_MIN, SENSOR_Z_MAX, g_poDataMonster->m_apJoinArray[3]->m_fPwmMin, g_poDataMonster->m_apJoinArray[3]->m_fPwmMax);
  }
  target[3]= iPwmValue;

  // Y maps to joint 4
  if(g_fZ >= 0 && g_fZ < 0.1){
    iPwmValue = mapfloat(g_fY, 0, (.1), g_poDataMonster->m_apJoinArray[4]->m_fPwmMin, g_poDataMonster->m_apJoinArray[4]->m_fPwmMax-50);  ///////<-hardcoded! 
  }
  else if(g_fZ >= 0.1 && g_fZ< 0.2){
    iPwmValue = mapfloat(g_fY, 0.1, 0.2, g_poDataMonster->m_apJoinArray[4]->m_fPwmMin + 50, g_poDataMonster->m_apJoinArray[4]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.2 && g_fZ< 0.3){
    iPwmValue = mapfloat(g_fY, 0.1, 0.2, g_poDataMonster->m_apJoinArray[4]->m_fPwmMin + 30, g_poDataMonster->m_apJoinArray[4]->m_fPwmMax);  ///////<-hardcoded!
  }
  else if(g_fZ >= 0.3 && g_fZ< 0.6){
    iPwmValue = mapfloat(g_fY, 0.3, 0.6, g_poDataMonster->m_apJoinArray[4]->m_fPwmMin, g_poDataMonster->m_apJoinArray[4]->m_fPwmMax);  ///////<-hardcoded!
  }
  else{
    iPwmValue = mapfloat(g_fY, SENSOR_X_MIN, SENSOR_X_MAX, g_poDataMonster->m_apJoinArray[4]->m_fPwmMin, g_poDataMonster->m_apJoinArray[4]->m_fPwmMax);

  }
  target[4]= iPwmValue;

  for (int i=0; i<5; i++){
    magnitude[i] = target[i] - location[i];
  }  
  for (int i=0; i<5; i++){
    force[i] = (magnitude[i] * springValue);
  }  
  for (int i=0; i<5; i++){
    velocity[i] = dampening * (velocity[i] + force[i]);
  }  
  for (int i=0; i<5; i++){
    location[i] = location[i] + velocity[i];
  }  
  
 ///// Set Dampening couter/reset
dampening = (dampening - 0.0005);
if (dampening < 0.6){
  dampening = 0.6;
}
if (g_bGotTweet == true){
 dampening = 1.0; 
}


  // Orient the robot towards the object
  g_poDataMonster->setPosture(location[0],  location[1],  location[2], location[3], location [4], g_bGotTweet);

#endif

}

// Robot calibration interface
void calibRobot()
{
  // Update the joint position every 100 draw cycles
  g_iJointUpdateTimerCounter++;
  if ( (g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT) == 0) {

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
    g_poDataMonster->moveJointToCalib(g_iJointSelect, g_iPwmValue);
  }
}

// Robot calibration interface
void carlosControlRobot()
{
  //print("Calibrating Robot\n");

  // Update the joint position every 100 draw cycles
  g_iJointUpdateTimerCounter++;
  if ( (g_iJointUpdateTimerCounter%JOINT_UPDATE_TIMER_COUNTER_LIMIT) == 0) {

    // Move the joint to the next PWM value
    g_poDataMonster->moveJoint(g_iJointSelect, g_iPwmValue);

    Serial.print("Controlingd | Joint: ");
    Serial.print(g_iJointSelect);
    Serial.print(" | Limit: LOW | PWM Value: ");
    Serial.println(g_iPwmValue);
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
      g_iPwmValue += 1;
      //g_fAngle += g_fOneDegInRad;
    } 
    else if (g_iByte == DOWN) {
      g_iPwmValue -= 1;
      //g_fAngle -= g_fOneDegInRad;
    }
    else if (g_iByte == LEFT) {

      g_iPwmValue = 5;
      //g_fAngle = 0;
      g_iJointCounter--;
    } 
    else if (g_iByte == RIGHT) {

      g_iPwmValue = 5;
      //g_fAngle = 0;
      g_iJointCounter++;
    }
  }

  if (g_iPwmValue < 0)
    g_iPwmValue = 0;
  if (g_iPwmValue > 255)
    g_iPwmValue = 255;

  if (g_iJointCounter < 0)
    g_iJointCounter = 0;
  if (g_iJointCounter > TOTAL_NUM_JOINTS-1)
    g_iJointCounter = TOTAL_NUM_JOINTS-1;

  g_iJointSelect = g_iJointCounter;
}

//////////
void blinkPin13()
{
  digitalWrite(g_iLed13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(g_iLed13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

///////////////////////////////
// Network
///////////////////////////////
void initNetwork(bool _bSetWiFi)
{

  if(_bSetWiFi)
  {
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present"); 
      return;
    } 

    String fv = WiFi.firmwareVersion();
    if( fv != "1.1.0" )
      Serial.println("Please upgrade the firmware");

    // attempt to connect to Wifi network:
  //  while ( status != WL_CONNECTED) { 
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(NETWORK_SSID);
      // Connect to WPA/WPA2 network:    
      status = WiFi.begin(NETWORK_SSID, NETWORK_PASS);

      // wait 10 seconds for connection:
      delay(10000);
   // }

    // you're connected now, so print out the data:
    Serial.print("You're connected to the network");
//    printCurrentNet();
//    printWifiData();
      printWifiStatus();

  }
  else
  {
    // Setup Ethernet
    // attempt a DHCP connection:
    Serial.println("Attempting to get an IP address using DHCP:");
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
    }

    /*
    if (!Ethernet.begin(mac)) {
     // if DHCP fails, start with a hard-coded address:
     Serial.println("failed to get an IP address using DHCP, trying manually");
     Ethernet.begin(mac, ip);
     }
     */
    Serial.print("My address:");
    Serial.println(Ethernet.localIP());
    // connect to Twitter:
    //connectToServer(_bSetWiFi);

  // give the network shield a second to initialize:
  delay(1000);

  }

  connectToServer(_bSetWiFi);


}
/*
String checkTwitter(bool _bSetWiFi)
{
  String sRetString = "";
  g_iTwitterPollCounter++;
  //if(g_iTwitterPollCounter%TWITTER_POLLING_TIME == 0) // Check Tweeter every ~5 seconds
  //if(g_iTwitterPollCounter == TWITTER_POLLING_TIME) // Check Tweeter every ~5 seconds
  //  if( (g_iTwitterPollCounter%TWITTER_POLLING_TIME) == 0) // Check Tweeter every ~5 seconds
  //  {
  //g_iTwitterPollCounter = 0;
  if(_bSetWiFi)
  {
    sRetString = checkTwitterWiFi(_bSetWiFi);
  }
  else
  {
    sRetString = checkTwitterEthernet(_bSetWiFi);
    //   Serial.println("*********************** HERE");
  }

  //  }

  return sRetString;
}
*/

// String g_sJsonString = "";
// boolean readingJsonString = false;

//String checkTwitterEthernet(bool _bSetWiFi)
String checkTwitter(bool _bSetWiFi)
{
  String sRetString = "";

  if (client.connected() || g_oWiFiClient.connected() ) {
    if (client.available() || g_oWiFiClient.available() ) {
      // read incoming bytes:
      char inChar;
      if(_bSetWiFi)
        inChar = g_oWiFiClient.read();
      else
        inChar = client.read();      
      
      // Debug
      //Serial.write(inChar);

      if ( inChar == '{' ) {
        // tweet is beginning. Clear the tweet string:
        readingJsonString = true;         
        g_sJsonString = "";
      }
      // if you're currently reading the bytes of a tweet,
      // add them to the tweet String:
      if (readingJsonString) {
        if (inChar != '}') {
          g_sJsonString += inChar;
        } 
        else {
          // if you got a "<" character,
          // you've reached the end of the tweet:
          g_sJsonString += '}';
          readingJsonString = false;
          sRetString = g_sJsonString;
          Serial.println(g_sJsonString);   
          // close the connection to the server:
          //client.stop(); 
          if(_bSetWiFi)
            g_oWiFiClient.stop();
          else
            client.stop(); 
        }
      }
    }   
  }
  else if (millis() - g_iLastAttemptTime > g_iRequestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer(_bSetWiFi);
  }

  return sRetString;
}

void connectToServer(bool _bSetWiFi) {

  if(_bSetWiFi)
  {

    // WiFi Version
    Serial.println("WiFi Starting connection to server...");
    // if you get a connection, report back via serial:
    if (g_oWiFiClient.connect(server, 80)) {
      Serial.println("WiFi connected to server");
      // Make a HTTP request:
      g_oWiFiClient.println(g_sHttpRequest); // ThingsSpeak
      g_oWiFiClient.println();
    }
    else
    {
        Serial.println("WiFi NOT connected to server"); 
    }
  }
  else
  {

    // attempt to connect, and wait a millisecond:
    Serial.println("Ethernet connecting to server...");
    if (client.connect(server, 80)) {
      Serial.println("making HTTP request...");
      // make HTTP GET request to twitter:
      client.println(g_sHttpRequest); // ThingsSpeak
      client.println();
    }
    else
    {
        Serial.println("Ethernet NOT connected to server"); 
    }
  }
  //#endif

  // note the time of this connect attempt:
  g_iLastAttemptTime = millis();

}

String checkTwitterWiFi(bool _bSetWiFi)
{

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//////////////////// WIFI ///////////////////////////////////
void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
  Serial.println();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



