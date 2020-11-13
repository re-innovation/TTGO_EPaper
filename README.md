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

### Open Weather Map example



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

