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

Some examples have been included in this repository:

* Connect to Open Weather Map and display weather data
* Display a random quotation
* Display the UK grid frequency

The examples are all bundled into one code example. This saves repeating any changes for the E-Paper display and the Wifi implementation. Hopefully from the examples it can be seen different approaches to equiring and responding to data. These are for example only.

### Sort out Arduino IDE to upload data to the ESP32:

You will need to include the ESP32 into your board manager on the Arduino IDE.
Ensure ESP32 is installed:

Put: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json in your preferences.

Then Tools -> Boards -> Board Manager. Click on Board Manager and search for "esp32" and install.

Please follow the instructions here:

https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

When uploading to the board use the "ESP32 Dev Module" option as the type of board.

### Include the correct libraries for the examples to compile:

For these examples you will need to include the following libraries:

These can be installed via Arduino IDE Library Manager:

* WiFiManager by tzapu            https://github.com/tzapu/WiFiManager  (version: 2.0.15-rc-1) (via Library Manager)
* ArduinoJson.h                   https://github.com/bblanchon/ArduinoJson  (version 6.20.1) (via Library Manager)
* TimeLib.h                       https://github.com/PaulStoffregen/Time (via Library Manager)
* AdaFruit_GFX.h                  https://github.com/adafruit/Adafruit-GFX-Library (version 1.11.5) (and dependancy Adafruit BusIO (14.1)  (via Library Manager)
* Adafruit_MQTT by Adafruit       Only needed if you want to get data from Adafruit IO.   (via Library Manager)

Also install:

* GxEPD2                          Download from here: [https://github.com/ZinggJM/GxEPD2](https://github.com/ZinggJM/GxEPD2)
Then install using library -> add ZIP library OR search for GxEPD2 repository in the library manager and install

### Setting up WiFi in all examples

The examples all use the WiFiManager to create an access point (AP) for adding your WiFi details. The wifi details are stored and used, unless it cannot connect.
The screen will show what to do, but at first switch on you need to connect to the access point created (details put onto E-Paper screen along with password). Configure the WiFi and click on your SSID and add your password. The unit will then restart and attempt to connect and get data.

You can always reset the details and put the unit into configuration mode by pressing the small button on the PCB labelled "IO39" - this puts unit into configuration mode for 120 seconds.

### Open Weather Map example

Upload the example to your E-Paper screen unit via a micro-USB cable, using the Arduino IDE.

You need to create an account with Open Weather Map (https://openweathermap.org/).

DEPRECIATED: Details for connection are here: https://openweathermap.org/api/one-call-api

You need to register and get an API key. With their free level you are limited to a certain number of API calls (1,000,00 a month or 60 per minute!), but for this application the free level is fine.

You can add your parameters when configuring the WiFi via the config portal. They are stored in EEPROM, but can be updated by changing in the WiFi config portal.

You need to set the following parameters (set your location according to Open Weather Map locations)

```
OWM_KEY       "YOUR OPEN WEATHER MAP KEY"                  // See: https://openweathermap.org/
OWM_SERVER    "api.openweathermap.org"
MY_CITY        "Nottingham"                   // Your home city See: http://bulk.openweathermap.org/sample/
MY_COUNTRY     "GB"                           // Your country
MY_LANGUAGE    "EN"                            // NOTE: Only the weather description (not used) is translated by OWM
// Arabic (AR) Czech (CZ) English (EN) Greek (EL) Persian(Farsi) (FA) Galician (GL) Hungarian (HU) Japanese (JA)
// Korean (KR) Latvian (LA) Lithuanian (LT) Macedonian (MK) Slovak (SK) Slovenian (SL) Vietnamese (VI)
MY_HEMISPHERE  "north"                         // or "south"
MY_UNITS       "M"                             // Use "M" for Metric or I for Imperial
MY_TIMEZONE    "GMT0BST,M3.5.0/01,M10.5.0/02"  // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
```

### Quotation Unit example

This example does not need any user info. Just upload this code varion. Then switch on your e-ink display and connect to the WiFi access point, as per the on-screen instructions.
When you enter your wifi password and SSID then it will restart and download a random quote. A new quote will appear each time the touch pad is pressed.

This code uses the following website, https://forismatic.com/en/, which returns a quote and the author as a line of text.

The final URL used to send via an http GET request is: http://api.forismatic.com/api/1.0/?method=getQuote&key=&format=text&lang=en

### Grid Frequency example



## More information

This example code is designed to be run on a TTGO T5 V2.3 2.13 Inch E-Paper Screen from Lilygo:

http://www.lilygo.cn/products.aspx

And a github repository for their info and notes is here:

https://github.com/Xinyuan-LilyGO/T5-Ink-Screen-Series

See more information in these blog posts:

https://www.re-innovation.co.uk/blog/2020/ttgo-e-paper-display/

https://www.re-innovation.co.uk/docs/creating-a-weather-display/


## Updates:

### Problems Encountered:

In these next sections I'm highlighting the code changes required to get this project going from the previous code that I had written. This is for information only, as it has been a while since I have developed any code the E-Paper screen.

### Problem: Library Update:

The libraries for the E-Paper unit have been updated and my code (from 2024) no longer compiles correctly. I decided the make the changes required to go from using GxEPD to using GxEPD2. LilyGo (who manufacture the ESP32 E-Ink board) have some updated example code to use which I needed as the starting point for updating: https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series/tree/master

The changes were not too bad and using the example code for LilyGo I was able to update the code so that the screen would display some information.

### Problem: Touch Wake Up:

The next issue was that the espressif libraries for setting up a touch pad had also changed. So I needed to change the code for going to sleep. Instead of:

&nbsp;touchAttachInterrupt(WAKE_UP_PIN, callback, THRESHOLD);
&nbsp;esp_sleep_enable_touchpad_wakeup();

I needed to use:

touchSleepWakeUpEnable(WAKE_UP_PIN, THRESHOLD);

This works and the unit will wake up from the touch input.

But the routine for returning the GPIO pin that has been triggered with the touch does not seem to work. I use:

&nbsp; touch_pad_t touchPin;
&nbsp; touchPin = (touch_pad_t)esp_sleep_get_touchpad_wakeup_status();

This works fine, but always returns '512', rather than the 0-9 which should enumerate to the different GPIO pins. This is still not fixed, if anyone has any solutions?

### Problem: Curious Electric logo is mirrored

Previously I drew a small black and white Curious Electric logo while the unit powered up. I was using the GxEPD library, but I have now updated to the GxEPD2 library. The original logo was produced as a mirror image so it would display via GxEPD. This must have been reversed in the new GxEDP2. I recreated the logo data using Image2LCD (see my original blog post here), but did not mirror the image (left to right). I then included that data within my icons page within the code. This all worked!

### Problem: Alignment of Text

There must have been a change in the origin setpoint in GxEPD2 as all my text was mis-aligned. I think the change was that rather than using the top left of the text as the origin, the code now uses the bottom left of the text as the origin. So I needed to push down all the text by 10 pixels.
