#ifndef TWITTER_MODULE_CPP
#define TWITTER_MODULE_CPP

#include "Arduino.h"


class TwitterModule{

  // Member
public:


private:

  // Button Code
  int m_iButtonReading;
  int m_iButtonPin;
  int m_iPrevButtonReading;
  long m_fTime;
  long m_fDebounce;


  // Methods

public:
  //Constructor
  TwitterModule(){

    // Initilize button
    m_iButtonReading = LOW;
    m_iButtonPin = 2;
    m_iPrevButtonReading = LOW;
    m_fTime = 0;
    m_fDebounce = 200;

    pinMode(m_iButtonPin, INPUT);

  }

  void update()
  {

  }

  bool gotTweet()
  {

    return updateButton();
  }

private:
  bool updateButton()
  {
    bool bRet = LOW;
    m_iButtonReading = digitalRead(m_iButtonPin);

    // if the input just went from LOW and HIGH and we've waited long enough
    // to ignore any noise on the circuit, toggle the output pin and remember
    // the time
    if (m_iButtonReading == HIGH && m_iPrevButtonReading == LOW && millis() - m_fTime > m_fDebounce) 
    {
      // Get debounce time
      m_fTime = millis();    
      bRet = HIGH;
    }

    // Update button state
    m_iPrevButtonReading = m_iButtonReading;

  Serial.println(m_iButtonReading, DEC);

    return bRet;
  }

};

#endif // TWITTER_MODULE_CPP










