#ifndef TWITTER_MODULE_CPP
#define TWITTER_MODULE_CPP

#include "Arduino.h"
#include "aJSON.h"
//#include "C:\arduino-1.5.3\libraries\aJson\src\aJSON.h"
//#include "..\..\.\libraries\aJson\src\aJSON.h"

// JSON channel data
// {"created_at":"2013-09-18T19:00:11-07:00","entry_id":5,"field1":"10"}

class TwitterModule{

  // Member
public:


private:

  // Button Code
  int m_iButtonReading;
  int m_iPrevButtonReading;
  long m_fButtonTime;
  long m_fDebounce;
  int m_iButtonPin;
  int m_iLedPin;

  int m_iLedTime;
  int m_iLedTimeOut;

  String m_sPrevTimeStamp;
  bool m_bRecevingChars;

  // Methods

public:
    //Constructor
  TwitterModule(int _iButtonPin, int _iLedPin){

    // Initilize button
    m_iButtonReading = LOW;
    m_iPrevButtonReading = LOW;
    m_iButtonPin = _iButtonPin;
    m_fButtonTime = 0;
    m_fDebounce = 200;
    pinMode(m_iButtonPin, INPUT);

    // Initialize Led
    m_iLedPin = _iLedPin;
    m_iLedTime = 0;
    m_iLedTimeOut = 200;
    pinMode(m_iLedPin, OUTPUT); 
    digitalWrite(m_iLedPin, LOW);  

    m_bRecevingChars = true;

  }

  bool gotWiFiTweet()
  {
    bool bRet = false;
    // Read data from the server
    // Connect to the server
    // Get next char from the server

    char* sJsonPacket = "{\"created_at\":\"2013-09-18T19:00:11-07:00\",\"entry_id\":5,\"field1\":\"10\"}";


    // Set to true when done receiving JSON packet
    m_bRecevingChars = true;

    // Process received packet
    if( !m_bRecevingChars )
    {
      // Parsing JSON content and getting the creation time
      aJsonObject* jsonObject = aJson.parse(sJsonPacket);
      aJsonObject* oCreated_at = aJson.getObjectItem(jsonObject, "created_at"); // Can get object for "root" but not parsed "jsonObject"
      if (oCreated_at == NULL) {
        Serial.println("ERROR: Couldn't find JSON item: \"created_at\"");
        return false;
      }

      // Check if we have a new Tweet
      // IF received full message
      String sCurTimeStamp = oCreated_at->valuestring;

      // Check with previous Tweet timestamp
      if( m_sPrevTimeStamp.compareTo(sCurTimeStamp) != 0 )  
      {
        //       Serial.println("GOT NEW TWEET");
        // The Tweet is new
        bRet = true;
      }

      // Update time stamp
      m_sPrevTimeStamp = sCurTimeStamp;

      // Start listening for new JSON packets
      m_bRecevingChars = true;
    }

    return bRet;
  }

  bool gotTweet()
  {

    // Check if we received a Tweet from the web or the physical button
    bool bGotWiFiTweet = gotWiFiTweet();
    bool bGotTweet = gotButtonTweet() || bGotWiFiTweet;
    updateTweetLed(bGotTweet);

    return bGotTweet;
  }

private:
  bool gotButtonTweet()
  {
    bool bRet = LOW;
    m_iButtonReading = digitalRead(m_iButtonPin);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (m_iButtonReading == HIGH && m_iPrevButtonReading == LOW && millis() - m_fButtonTime > m_fDebounce) 
    {
      // Get debounce time
      m_fButtonTime = millis();    

      // Set state
      bRet = HIGH;
    }

    // Update button state
    m_iPrevButtonReading = m_iButtonReading;

    // Serial.println(m_iButtonReading, DEC);

    return bRet;
  }

  void updateTweetLed(bool _bLedState)
  {
    if( _bLedState )
    {
      digitalWrite(m_iLedPin, HIGH);
    }
    else
    {
      if( millis() - m_iLedTime > m_iLedTimeOut )
      {
        digitalWrite(m_iLedPin, LOW);
        m_iLedTime = millis();
      }
    } 
  }

};

#endif // TWITTER_MODULE_CPP



















