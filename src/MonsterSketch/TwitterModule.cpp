#ifndef TWITTER_MODULE_CPP
#define TWITTER_MODULE_CPP

#include "Includes.h"
#include "aJSON.h"

class TwitterModule{

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

  int m_iNetworkStatusLedPin;
  char m_cJsonPacket[JSON_BUFFER_SIZE];

  // Methods
public:
  //Constructor
  TwitterModule(int _iButtonPin, int _iLedPin, int _iNetLedPin){

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

    // Initialize WiFi
    m_iNetworkStatusLedPin = _iNetLedPin;
    pinMode(m_iNetworkStatusLedPin, OUTPUT); 
    digitalWrite(m_iNetworkStatusLedPin, LOW);  
  }

  bool gotStringTweet(String _sJsonString)
  {
    bool bRet = false;
    
    // Making sure the string is not greater than the buffer
    if( _sJsonString.length() <= 0 || JSON_BUFFER_SIZE < _sJsonString.length()+1 )
      return bRet;
     
    // Making sure it is JSON
    if( !_sJsonString.startsWith("{") || !_sJsonString.endsWith("}") )
      return bRet;
      
     // Format the string to parse it (WE ARE ADDING +1 characters below. This can cause a problem if we receive more than BUFFER allows)
    _sJsonString.toCharArray(m_cJsonPacket,_sJsonString.length()+1);

    // Parsing JSON content and getting the creation time
    aJsonObject* jsonObject = aJson.parse(m_cJsonPacket);
    if (jsonObject == NULL) {
      Serial.println("ERROR: Couldn't parser the JSON string.");
      return false;
    }

    aJsonObject* oCreated_at = aJson.getObjectItem(jsonObject, "created_at"); // Can get object for "root" but not parsed "jsonObject"
    Serial.println(oCreated_at->valuestring);
    if (oCreated_at == NULL) {
      Serial.println("ERROR: Couldn't find JSON item: \"created_at\"");
      return false;
    }

    // Check if we have a new Tweet. Compare current with previous datapoint time stamp.
    String sCurTimeStamp(oCreated_at->valuestring);
    
    if( m_sPrevTimeStamp.compareTo(sCurTimeStamp) != 0 )  // 0 == Strings are the same
    {
      Serial.println("GOT WEB TWEET");
      // The Tweet is new
      bRet = true;
    }

    // Update time stamp
    m_sPrevTimeStamp = sCurTimeStamp;

    // Start listening for new JSON packets
    m_bRecevingChars = true;

    return bRet;
  }

  bool gotTweet(String _sJsonString)
  {

    // Check if we received a Tweet from the web or the physical button
    bool bGotStrTweet = gotStringTweet(_sJsonString);
    bool bGotTweet = gotButtonTweet() || bGotStrTweet;
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
     Serial.println("GOT BUTTON TWEET");
      
      // Get debounce time
      m_fButtonTime = millis();    

      // Set state
      bRet = HIGH;
    }

    // Update button state
    m_iPrevButtonReading = m_iButtonReading;

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



























