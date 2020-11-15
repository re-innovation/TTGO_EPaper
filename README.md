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
* Copy the files from the folder called "src_cpp"
* Paste them into the folder called "src".

This is because these examples use the older c++ (cpp) files.

To get the E-Paper display working I also needed to ensure I had the correct driver src installed. This is called "GxGDE0213B72B".
When you have installed the library, for the GxEPD library you also need to:
* Got to the folder where the library has been installed (your sketchbook -> libraries).
* Copy the "GxGDE0213B72B" folder from this repository to GxEPD -> src  and paste there.


### Open Weather Map example

You need to create an account with Open Weather Map (https://openweathermap.org/).

Details for connection are here:https://openweathermap.org/api/one-call-api

You need to register and get an API key. With their free level you are limited to a certain number of API calls (1,000,00 a month or 60 per minute!), but for this application the free level is fine.

Within the Config.h file you need to include:
```
#define   OWM_KEY       "YOUR OPEN WEATHER MAP KEY"                  // See: https://openweathermap.org/
#define   OWM_SERVER    "api.openweathermap.org"
```

And also set your location according to Open Weather Map locations
```
#define   MY_CITY        "Nottingham"                   // Your home city See: http://bulk.openweathermap.org/sample/
#define   MY_COUNTRY     "GB"                           // Your country
#define   MY_LANGUAGE    "EN"                            // NOTE: Only the weather description (not used) is translated by OWM
// Arabic (AR) Czech (CZ) English (EN) Greek (EL) Persian(Farsi) (FA) Galician (GL) Hungarian (HU) Japanese (JA)
// Korean (KR) Latvian (LA) Lithuanian (LT) Macedonian (MK) Slovak (SK) Slovenian (SL) Vietnamese (VI)
#define   MY_HEMISPHERE  "north"                         // or "south"
#define   MY_UNITS       "M"                             // Use "M" for Metric or I for Imperial
#define   MY_TIMEZONE    "GMT0BST,M3.5.0/01,M10.5.0/02"  // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
```

Change these values and then upload using the Arduino IDE.

### Adafruit IO example

This example shows how to read a data feed from your own AdafruitIO feeds (https://io.adafruit.com/). You will need to make an account with AdafruitIO (free for up to 5 feeds).

I have already set up a feed from a radiation sensor in my workshop. 
You will need to change a few thuings to get your feed running.

First you need to include your AdafruitIO username and key into the config file:
```
#define AIO_SERVER          "io.adafruit.com"       // direct
#define AIO_SERVERPORT      1883                    // 8883  // Use 8883 if at all possible!
#define AIO_USERNAME        "YOUR USER NAME"        // This is your Adafruit IO username
#define AIO_KEY             "YOUR KEY"              // This is your Adafruit IO Key
```
You then need to set up two feed. One is to subscribe to the feed and the other is publish to feed. As MQTT does not automatically give back a result unless it changes, AdafruitIO has a special case that when we wake up we need to publish (0) to the feed. This will send a message back to any feeds subscribed to the channel, which will give us the latest value. Thats why we need to subscribe and publish with a get here. In these lines change "airradiation" to the name of your feed channel.
```
Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/airradiation");
Adafruit_MQTT_Publish   getRadiation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/airradiation/get");
```
In the example code this returns a counts per minute value from the radiation sensor. This is displayed, along with the coverted micro-Sv value (which is just multiplied by 0.0057).
This value also is used to choose the icon to display. I have set three different icons: Low, Medium and High. These were drawn in inkscape and then converted to a 1-bit jpg and then converted to the data file required. Please see my blog post about how I did this, if you are interested (https://www.re-innovation.co.uk/blog/2020/ttgo-e-paper-display/).

This example code can either be set to wake with a touch press or on a timer. I'm using a timer to check every 2 hours... 

### Quotation Unit example

This example does not need any user info. Just upload this code. Then switch on your e-ink display and connect to the WiFi access points, as per the on-screen instructions.
When you enter your wifi password and SSID then it will restart and download a random quote. A new quote will appear each time the touch pad is pressed.

This code uses the following website, https://forismatic.com/en/, which returns a quote and the author as a line of text.

The final URL used to send via an http GET request is: http://api.forismatic.com/api/1.0/?method=getQuote&key=&format=text&lang=en

## More information

This example code is designed to be run on a TTGO T5 V2.3 2.13 Inch E-Paper Screen from Lilygo:

http://www.lilygo.cn/products.aspx

And a github repository for their info and notes is here:

https://github.com/Xinyuan-LilyGO/T5-Ink-Screen-Series

See more information in these blog posts:

https://www.re-innovation.co.uk/blog/2020/ttgo-e-paper-display/

https://www.re-innovation.co.uk/docs/creating-a-weather-display/

