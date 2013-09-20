#ifndef TWITTER_MODULE_CPP
#define TWITTER_MODULE_CPP

#include "Arduino.h"


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

  }

  bool gotWiFiTweet()
  {

    return false;
  }

  bool gotTweet()
  {

    // Check if we received a Tweet from the web or the physical button
    bool bGotTweet = gotButtonTweet() || gotWiFiTweet();
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













