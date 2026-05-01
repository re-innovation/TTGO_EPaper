#include "Config.h"

#ifdef GRIDFREQ_EXAMPLE
#include "gridfreq.h"
#include <TimeLib.h>  // Using the Arduino Library Manager, install "Time by Michael Margolis".

bool printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    return (false);
  }
  Serial.println(&timeinfo, "%B %d %Y %H:%M:%S");
  // Serial.print(F("Day of week: "));
  // Serial.println(&timeinfo, "%A");
  // Serial.print(F("Month: "));
  // Serial.println(&timeinfo, "%B");
  // Serial.print(F("Day of Month: "));
  // Serial.println(&timeinfo, "%d");
  // Serial.print(F("Year: "));
  // Serial.println(&timeinfo, "%Y");
  // Serial.print(F("Hour: "));
  // Serial.println(&timeinfo, "%H");
  // Serial.print(F("Hour (12 hour format): "));
  // Serial.println(&timeinfo, "%I");
  // Serial.print(F("Minute: "));
  // Serial.println(&timeinfo, "%M");
  // Serial.print(F("Second: "));
  // Serial.println(&timeinfo, "%S");
  // Serial.println(F("Time variables"));
  // char timeHour[3];
  // strftime(timeHour,3, "%H", &timeinfo);
  // Serial.println(timeHour);
  // char timeWeekDay[10];
  // strftime(timeWeekDay,10, "%A", &timeinfo);
  // Serial.println(timeWeekDay);
  // Serial.println();
  return (true);
}

String returnTimeDisplay() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    return ("");  // Just return blank
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  char buffer[64];  // Large enough to hold your formatted string
  strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M", &timeinfo);
  // Convert the char array to an Arduino String object
  String timeStr = String(buffer);

  return (timeStr);
}

String returnTimeNow(int _sec_offset) {
  // Create output looking like:"2026-04-16T13%3A00%3A00Z";
  // Need to convert time into UTC, then remove offset, then re-create the time:
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    return ("NO TIME");
  }

  time_t _time_unix;
  _time_unix = mktime(&timeinfo);
  time_t _time_past = _time_unix - _sec_offset;

  // Serial.print("Time unix in seconds:\t");
  // Serial.println(_time_unix);
  // Serial.print("Time unix minus offset:\t");
  // Serial.println(_time_past);

  struct tm *returntimeinfo;
  returntimeinfo = localtime(&_time_past);
  char timeReturned[50];
  strftime(timeReturned, sizeof(timeReturned), "%Y-%m-%dT%H:%M:%SZ", returntimeinfo);
  return ((String)timeReturned);
}

#endif