# TTGO_EPaper
Files for getting started with the TTGO EPaper Display unit with ESP-32 and Arduino IDE.

Plêase see https://www.curiouselectric.co.uk/products/e-paper-display-unit for kit of parts.

Need a portable, low-energy, wireless display for the data you are collecting?

This display unit has an ESP32 Wi-Fi enabled microcontroller and a 2.13” e‑ink display, along with a nice wooden enslosure.

## Enclosure design files

The .svg design files for the 3mm ply wooden enclosure and the 1mm acrylic spacers are available here:

https://github.com/re-innovation/TTGO_EPaper/tree/master/TTGO_EINK_ENCLOSURE

These have been designed in Inkscape and laser cut with Lightburn software.

## Examples

Three main examples have been included in this repository:
* Connect to Open Weather Map and display weather data
* Connect to an Adafruit IO feed and display the data
* Display a random quotation 

You will need to include the ESP32 into your board manager on the Arduino IDE.
Please follow the instructions here:
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

For these examples you will need to include the following libraries:
* ESP_WiFiManager by Khoi Howang
* GxEPD by Jean-Marc Zingg

These can be installed via Arduino IDE Library Manager.

When you have installed the library, for the ESP_WiFiManager library you also need to:
* Got to the folder where the library has been installed (your sketchbook -> libraries).
* Copy the files from src_cpp and paste them into src.
This is because these examples use the older c++ (cpp) files.

### Open Weather Map example

You need to create an account with Open Weather Map (https://openweathermap.org/).

Details for connection are here:https://openweathermap.org/api/one-call-api

You need to register and get an API key. With their free level you are limited to a certain number of API calls (1,000,00 a month or 60 per minute!), but for this application the free level is fine.

Within the Config.h file you need to include:

#define   OWM_KEY       "YOUR OPEN WEATHER MAP KEY"                  // See: https://openweathermap.org/
#define   OWM_SERVER    "api.openweathermap.org"

And also set your location according to Open Weather Map locations

#define   MY_CITY        "Nottingham"                   // Your home city See: http://bulk.openweathermap.org/sample/
#define   MY_COUNTRY     "GB"                           // Your country
#define   MY_LANGUAGE    "EN"                            // NOTE: Only the weather description (not used) is translated by OWM
// Arabic (AR) Czech (CZ) English (EN) Greek (EL) Persian(Farsi) (FA) Galician (GL) Hungarian (HU) Japanese (JA)
// Korean (KR) Latvian (LA) Lithuanian (LT) Macedonian (MK) Slovak (SK) Slovenian (SL) Vietnamese (VI)
#define   MY_HEMISPHERE  "north"                         // or "south"
#define   MY_UNITS       "M"                             // Use "M" for Metric or I for Imperial
#define   MY_TIMEZONE    "GMT0BST,M3.5.0/01,M10.5.0/02"  // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

Change these values and then upload using the Arduino IDE.

### Adafruit IO example



### Quotation Unit example

I found the following website, https://forismatic.com/en/, which returns a quote and the author as a line of text.

I set about figuring out the http request to send and return a quote in a variety of formats (text, json, xml). This was helped through the use of their API help pages.

The final URL I used to send via an http request is: http://api.forismatic.com/api/1.0/?method=getQuote&key=&format=text&lang=en


## More information

This example code is designed to be run on a TTGO T5 V2.3 2.13 Inch E-Paper Screen from Lilygo:

http://www.lilygo.cn/products.aspx

And a github repository for their info and notes is here:

https://github.com/Xinyuan-LilyGO/T5-Ink-Screen-Series

See more information in these blog posts:

https://www.re-innovation.co.uk/blog/2020/ttgo-e-paper-display/

https://www.re-innovation.co.uk/docs/creating-a-weather-display/

