#include "utilities.h"
#include "time.h"            // Built-in
#include <TimeLib.h>  // Include time functions

/*
  Method to print the reason by which ESP32
  has been awaken from sleep
*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

/*
  Method to print the touchpad by which ESP32
  has been awaken from sleep
*/
void print_wakeup_touchpad() {
  
  touch_pad_t touchPin;
  touchPin = esp_sleep_get_touchpad_wakeup_status();
  switch (touchPin)
  {
    case 0  : Serial.println("Touch detected on GPIO 4"); break;
    case 1  : Serial.println("Touch detected on GPIO 0"); break;
    case 2  : Serial.println("Touch detected on GPIO 2"); break;
    case 3  : Serial.println("Touch detected on GPIO 15"); break;
    case 4  : Serial.println("Touch detected on GPIO 13"); break;
    case 5  : Serial.println("Touch detected on GPIO 12"); break;
    case 6  : Serial.println("Touch detected on GPIO 14"); break;
    case 7  : Serial.println("Touch detected on GPIO 27"); break;
    case 8  : Serial.println("Touch detected on GPIO 33"); break;
    case 9  : Serial.println("Touch detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}

//##############################################################################
String digitalClockDisplay(long int _timeUTC)
{
  // This converts the UTC in seconds to a string to display.

  time_t t = _timeUTC; // store the current time in time variable t
  // Create a String of date and time to display
  String _displayDateTime = String(hour(t)) + ":";
  if (minute(t) < 10)
  {
    _displayDateTime = _displayDateTime + "0" + String(minute(t));
  }
  else
  {
    _displayDateTime = _displayDateTime + String(minute(t));
  }
  //_displayDateTime = _displayDateTime + " on " + String(day(t)) + "/" + String(month(t)) + "/" + String(year(t));
  _displayDateTime = _displayDateTime + " on " + String(day(t)) + "/" + String(month(t));
  return _displayDateTime;
}

//#########################################################################
String findHours(String _period)
{
  // This find the time from the period string
  int _startColon = _period.indexOf(" ");
  int _firstColon = _period.indexOf(":");
  int _secondColon = _period.lastIndexOf(":");
  String _hourString = _period.substring(_startColon + 1, _firstColon);
  String _minString = _period.substring(_firstColon + 1, _secondColon);
  String _displayTime = _hourString;
  return (_displayTime);
}

//#########################################################################
String findDate(String _period)
{
  // This find the time from the period string
  int _startColon = _period.indexOf(" ");
  String _dateString = _period.substring(0, _startColon);
  return (_dateString);
}



//######################################################################

String addTimeZone(String _displayHours, int _timeZone)
{
  int _displayInt = _displayHours.toInt();
  _displayInt = _displayInt + (_timeZone / 3600); // 3600 seconds in hours
  String _returnedHours = String(_displayInt);
  if (_displayInt < 10)
  {
    _returnedHours = "0" + _returnedHours;
  }
  return (_returnedHours);
}
