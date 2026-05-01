
#include "display.h"
#include "utilities.h"

#ifdef WEATHER_EXAMPLE
#include "weather.h"
#endif

#ifdef GRIDFREQ_EXAMPLE
#include "gridfreq.h"
#endif

// UPDATED 14/4/2026:
// For Eink Paper - Use GxEPD2 Library
// include library, include base class, make path known
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>

#include "CE_Icons.h"  // Curious Electric Icons

// GxIO_Class io(SPI, ELINK_SS, ELINK_DC, ELINK_RESET);
// GxEPD_Class display(io, ELINK_RESET, ELINK_BUSY);

// // or select the display class and display driver class in the following file (new style):
// #include "GxEPD2_display_selection_new_style.h"

// GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT>
//   //GxEPD2_BW<GxEPD2_290_GDEY029T94, displayHeight>
//   display(GxEPD2_290_GDEY029T94(/*CS=5*/ ELINK_SS, /*DC=*/ELINK_DC, /*RST=*/ELINK_RESET, /*BUSY=*/ELINK_BUSY));  // GDEY029T94  128x296, SSD1680, (FPC-A005 20.06.15)
// void displayTest() {
//   for (int n = 0; n < 20; n++) {
//     Serial.println("TEST");
//   }
// }

GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT>
  //GxEPD2_BW<GxEPD2_290_GDEY029T94, displayHeight>
  display(GxEPD2_213_B74(/*CS=5*/ ELINK_SS, /*DC=*/ELINK_DC, /*RST=*/ELINK_RESET, /*BUSY=*/ELINK_BUSY));  // GDEM0213B74 122x250, SSD1680
void displayTest() {
  for (int n = 0; n < 20; n++) {
    Serial.println("TEST");
  }
}


//#########################################################################
void displayShowCELogo() {
  // This routine shows the Curious Electric Logo on the screen. Always Useful!!
  //display.drawExampleBitmap(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, GxEPD_BLACK);
  //display.drawGrayscaleBitmap(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, GxEPD_BLACK);
  display.setFullWindow();

  display.firstPage();
  do {
#ifdef QUOTATION_EXAMPLE
    display.setCursor(0, 21);
    display.println("Finding");
    display.setCursor(0, 41);
    display.println("Quote...");
#endif
#ifdef WEATHER_EXAMPLE
    display.setCursor(0, 21);
    display.println("Finding...");
    display.setCursor(0, 41);
    display.println(owm_settings.MY_CITY);
    display.setCursor(0, 61);
    display.println(owm_settings.MY_COUNTRY);
#endif
#ifdef GRIDFREQ_EXAMPLE
    display.setCursor(0, 21);
    display.println("Obtaining");
    display.setCursor(0, 41);
    display.println("Data...");
#endif

    //display.drawBitmap(140, 0, gImage_CE_Logo_test_data_100x120, 100, 120, GxEPD_BLACK);
    display.drawInvertedBitmap(140, 2, gImage_CE_Logo_v2_100x120, 100, 120, GxEPD_BLACK);
    //display.drawImage(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, false, false, true);
  } while (display.nextPage());
}

//#########################################################################
void displayShowError() {
  // This routine shows the Curious Electric Logo on the screen. Always Useful!!
  //display.drawExampleBitmap(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, GxEPD_BLACK);
  //display.drawGrayscaleBitmap(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, GxEPD_BLACK);
  display.setFullWindow();

  display.firstPage();
  do {
    display.setCursor(0, 21);
    display.println("Data");
    display.setCursor(0, 41);
    display.println("Error...");
    //display.drawBitmap(140, 0, gImage_CE_Logo_test_data_100x120, 100, 120, GxEPD_BLACK);
    display.drawInvertedBitmap(140, 5, gImage_CE_Logo_v2_100x120, 100, 120, GxEPD_BLACK);
    //display.drawImage(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, false, false, true);
  } while (display.nextPage());
}

#ifdef QUOTATION_EXAMPLE
//#########################################################################
void displayQuote(String _quote, String _author) {
  // Need to check the size of the quote
  // If it is bigger than *** then need to chop into smaller strings and display parts
  // Display is 250px x 122px

  // This displays the quote on the e-ink display
  display.setTextSize(1);
  display.setFullWindow();
  display.firstPage();
  do {
    int _index = 0;
    String _local_string = _quote.substring(_index, _index + 22);
    int n = 20;  // First line placement
    //while (_local_string.length() > 0)
    for (int y = 0; y < 5; y++) {
      // Max 6 lines of text available...
      // Find the final space in the string:
      _local_string = _quote.substring(0, 22);
      _index = 22;
      while (!isSpace(_local_string[_index])) {
        _index--;
      }
      displayText(_quote.substring(0, _index), n, CENTER_ALIGNMENT);
      n = n + 17;  // Go to a new line
      int end_of_quote = _quote.length();
      _quote = _quote.substring(_index, end_of_quote);
      if (_quote.length() < 22) {
        displayText(_quote, n, CENTER_ALIGNMENT);
        break;
      }
    }
    displayText(_author, 121, CENTER_ALIGNMENT);
  } while (display.nextPage());
}
#endif

//#########################################################################
void displayInit() {

  if (isInit) {
    return;
  }
  isInit = true;                         // This only lets the init run once...
                                         //display.init();
  display.init(115200, true, 2, false);  // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display.setRotation(1);
  //display.eraseDisplay();
  display.setTextColor(GxEPD_BLACK);
  //  display.setFont(&DEFALUT_FONT);
  //  display.setTextSize(0);
  display.fillScreen(GxEPD_WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoBold9pt7b);
  //display.update();
}

void displayClear() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
}

void displayUpdatingScreen() {
  display.setFullWindow();
  display.firstPage();
  do {
    display.setCursor(0, 11);
    display.println("Finding");
    display.setCursor(0, 31);
    display.println("Quote...");
  } while (display.nextPage());
}

//#################################################################################
void displayText(const String &_str, int16_t _y, uint8_t _alignment) {
  int16_t x = 0;
  int16_t x1, y1;
  uint16_t w, h;
  display.setCursor(x, _y);
  display.getTextBounds(_str, x, _y, &x1, &y1, &w, &h);

  switch (_alignment) {
    case RIGHT_ALIGNMENT:
      display.setCursor(displayWidth - w - x1, _y);
      break;
    case LEFT_ALIGNMENT:
      display.setCursor(0, _y);
      break;
    case CENTER_ALIGNMENT:
      display.setCursor((displayWidth / 2) - ((w + x1) / 2), _y);
      break;
    default:
      break;
  }
  display.println(_str);
}

#ifdef WEATHER_EXAMPLE
//#########################################################################################
void displayWeatherInfo(Forecast_record_type _WxConditions[], Forecast_record_type _WxForecast[]) {
  // 2.13" e-paper display is 250 x 122 px resolution
  display.setFullWindow();
  display.firstPage();
  do {
    // Here want to display the time and date of most recent update:
    display.setCursor(0, 27);
    String displayDateTime = digitalClockDisplay(_WxConditions[0].TimeUTC + _WxConditions[0].TimeZone);
    display.println(displayDateTime);
    display.setCursor(0, 45);
    display.println(_WxConditions[0].Forecast0);
    display.setCursor(150, 21);
    display.println("Now:");
    // This shows the forecast for the day:
    displayWeatherIcon(200, 5, _WxConditions[0].Icon);  // Weather person depiction of weather

    // This shows the 3 hr forecast for next 5 lots of 3 hours
    displayWeatherIcon(0, 72, _WxForecast[0].Icon);  // Weather person depiction of weather
    display.setCursor(5, 70);
    String displayTime = addTimeZone(findHours(_WxForecast[0].Period), _WxConditions[0].TimeZone);
    display.println(displayTime);
    displayWeatherIcon(50, 72, _WxForecast[1].Icon);  // Weather person depiction of weather
    display.setCursor(55, 68);
    displayTime = addTimeZone(findHours(_WxForecast[1].Period), _WxConditions[0].TimeZone);
    display.println(displayTime);

    displayWeatherIcon(100, 72, _WxForecast[2].Icon);  // Weather person depiction of weather
    display.setCursor(105, 68);
    displayTime = addTimeZone(findHours(_WxForecast[2].Period), _WxConditions[0].TimeZone);
    display.println(displayTime);

    displayWeatherIcon(150, 72, _WxForecast[3].Icon);  // Weather person depiction of weather
    display.setCursor(155, 68);
    displayTime = addTimeZone(findHours(_WxForecast[3].Period), _WxConditions[0].TimeZone);
    display.println(displayTime);

    displayWeatherIcon(200, 72, _WxForecast[4].Icon);  // Weather person depiction of weather
    display.setCursor(205, 68);
    displayTime = addTimeZone(findHours(_WxForecast[4].Period), _WxConditions[0].TimeZone);
    display.println(displayTime);

    //  DisplayWxIcon(276, 15, WxConditions[0].Icon, LargeIcon); // Weather icon
    //  DisplayMainWeatherSection(0, 148);                       // Weather forecast text
    //  DisplayForecastSection(131, 172);                        // 3hr interval forecast boxes
    //  DisplayAstronomySection(131, 174);                       // Astronomy section Sun rise/set, Moon phase and Moon icon
    //  DisplayWindSection(50, 220, WxConditions[0].Winddir, WxConditions[0].Windspeed, 50); // Wind direction info
    //  DisplaySystemStatus(293, 238);

  } while (display.nextPage());
}

//#########################################################################################
void displayWeatherIcon(int x, int y, String IconName) {

  // TTGO display is: 250px x 122px
  // Icon needs to be inverted to use here???
  // uint16_t invert = GxEPD::bm_invert;
  //display.drawRect(x, y, 130, 130, GxEPD_BLACK);
  // NOTE: Using 'drawInvertedBitmap' and not 'drawBitmap' so that images are WYSIWYG, otherwise all images need to be inverted
  if (IconName == "01d" || IconName == "01n") display.drawInvertedBitmap(x, y, gImage_sunny, 50, 50, GxEPD_BLACK);
  else if (IconName == "02d" || IconName == "02n") display.drawInvertedBitmap(x, y, gImage_mostlySunny, 50, 50, GxEPD_BLACK);
  else if (IconName == "03d" || IconName == "03n") display.drawInvertedBitmap(x, y, gImage_cloudy, 50, 50, GxEPD_BLACK);
  else if (IconName == "04d" || IconName == "04n") display.drawInvertedBitmap(x, y, gImage_mostlySunny, 50, 50, GxEPD_BLACK);
  else if (IconName == "09d" || IconName == "09n") display.drawInvertedBitmap(x, y, gImage_chanceRain, 50, 50, GxEPD_BLACK);
  else if (IconName == "10d" || IconName == "10n") display.drawInvertedBitmap(x, y, gImage_rain, 50, 50, GxEPD_BLACK);
  else if (IconName == "11d" || IconName == "11n") display.drawInvertedBitmap(x, y, gImage_lightning, 50, 50, GxEPD_BLACK);
  else if (IconName == "13d" || IconName == "13n") display.drawInvertedBitmap(x, y, gImage_snow, 50, 50, GxEPD_BLACK);
  else if (IconName == "50d") display.drawInvertedBitmap(x, y, gImage_haze, 50, 50, GxEPD_BLACK);
  else if (IconName == "50n") display.drawInvertedBitmap(x, y, gImage_fog, 50, 50, GxEPD_BLACK);
  else display.drawInvertedBitmap(x, y, gImage_noData, 50, 50, GxEPD_BLACK);
}
#endif

#ifdef GRIDFREQ_EXAMPLE
void drawFreqGraph(int _data_points[], int _x, int _y, float _recent_f) {
  // 2.13" e-paper display is 250 x 122 px resolution
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);  // Clear buffer

    // Add horizontal grid lines:
    //display.drawLine(_x, _y, _x + displayWidth, _y, GxEPD_BLACK);
    int graph_line_pixels = (int)(GRAPH_SCALE / 10.0);
    writeDashedHLine(_x, _y + graph_line_pixels, _x + displayWidth, 10);
    writeDashedHLine(_x, _y + (graph_line_pixels * 2), _x + displayWidth, 10);
    writeDashedHLine(_x, _y, _x + displayWidth, 5);
    writeDashedHLine(_x, _y - graph_line_pixels, _x + displayWidth, 10);
    writeDashedHLine(_x, _y - (graph_line_pixels * 2), _x + displayWidth, 10);

    // Add text markers
    display.setCursor(_x, _y - 14);
    display.println("50.1");
    display.setCursor(_x, _y - 44);
    display.println("50.2");
    display.setCursor(_x, _y + 25);
    display.println("49.9");
    display.setCursor(_x, _y + 54);
    display.println("49.8");
    // Add vertical lines
    writeDashedVLine(0, _x + 62, displayHeight, 5);
    writeDashedVLine(0, _x + 125, displayHeight, 5);
    writeDashedVLine(0, _x + 188, displayHeight, 5);

    for (int x = 1; x < 144; x++) {
      float x_change = ((float)displayWidth / 144.0);
      int x_1 = (int)(x_change * ((float)x - 1.0));
      int x_2 = (int)(x_change * ((float)x));
      display.drawLine(_x + x_1, _y + _data_points[x - 1], _x + x_2, _y + _data_points[x], GxEPD_BLACK);
    }
    // Add Frequency now information:
    display.setCursor(_x + 110, _y + 54);
    display.println("Freq:");
    display.setCursor(_x + 170, _y + 54);
    if (_recent_f == -0.0) {
      display.println("NA");  // Just in case we did not get the data
    } else {
      display.println(_recent_f);
    }
    display.setCursor(_x + 230, _y + 54);
    display.println("Hz");

    // Add the time/date to the graph
    display.setCursor(_x + 85, 17);
    display.println(returnTimeDisplay());

    // Lets also show the trend for the past hour - is it + or -?
    // Then draw an arrow to display the last 10min trend
    // Last 10 min trend is just value 0 in the data array
    int min_trend = _data_points[0];  // This is an int which has been scaled from the float original

    // Actual freq is 10_min_trend * GRAPH_SCALE, but we dont need to know that really.
    // If 10_min_trend -> 31->60 (50.11 to 50.2) then large INCREASE
    // If 10_min_trend -> 6->30 (50.02 to 50.1) then small INCREASE
    // If 10_min_trend -> -6->6 (49.98 to 50.02) then stable
    // If 10_min_trend -> -6->-30 (49.9 to 49.8) then small REDUCE
    // If 10_min_trend -> -31->-60 (49.98 to 49.9) then large REDUCE
    //display.fillTriangle(displayWidth / 2 - 40, displayHeight / 2 - 30, displayWidth / 2 + 40, displayHeight / 2 - 30, displayWidth / 2, displayHeight / 2 + 30, GxEPD_BLACK);  // large down
    //display.fillTriangle(displayWidth / 2 - 20, displayHeight / 2 - 15, displayWidth / 2 + 20, displayHeight / 2 - 15, displayWidth / 2, displayHeight / 2 + 15, GxEPD_BLACK); // small down
    //display.fillTriangle(displayWidth / 2 - 40, displayHeight / 2 + 30, displayWidth / 2 + 40, displayHeight / 2 + 30, displayWidth / 2, displayHeight / 2 - 30, GxEPD_BLACK); // large up
    //display.fillTriangle(displayWidth / 2 - 20, displayHeight / 2 + 15, displayWidth / 2 + 20, displayHeight / 2 + 15, displayWidth / 2, displayHeight / 2 - 15, GxEPD_BLACK);  // small up
    // display.fillRect(displayWidth / 2 - 20 , displayHeight / 2 - 8, 40, 5, GxEPD_BLACK);
    // display.fillRect(displayWidth / 2 - 20 , displayHeight / 2 + 4, 40, 5, GxEPD_BLACK);
    Serial.print("10 Min Value=");
    Serial.println(min_trend);

    if (min_trend > 31) {
      //large INCREASE
      display.fillTriangle(displayWidth / 2 - 40, displayHeight / 2 + 30, displayWidth / 2 + 40, displayHeight / 2 + 30, displayWidth / 2, displayHeight / 2 - 30, GxEPD_BLACK);  // large up
    } else if (min_trend > 6 && min_trend < 30) {
      //small INCREASE
      display.fillTriangle(displayWidth / 2 - 20, displayHeight / 2 + 15, displayWidth / 2 + 20, displayHeight / 2 + 15, displayWidth / 2, displayHeight / 2 - 15, GxEPD_BLACK);  // small up
    } else if (min_trend < 6 && min_trend >-6) {
      //stable
      display.fillRect(displayWidth / 2 - 20, displayHeight / 2 - 8, 40, 5, GxEPD_BLACK);
      display.fillRect(displayWidth / 2 - 20, displayHeight / 2 + 4, 40, 5, GxEPD_BLACK);
    } else if (min_trend < -6 && min_trend > -30) {
      //small REDUCE
      display.fillTriangle(displayWidth / 2 - 20, displayHeight / 2 - 15, displayWidth / 2 + 20, displayHeight / 2 - 15, displayWidth / 2, displayHeight / 2 + 15, GxEPD_BLACK);  // small down
    } else if (min_trend < -30) {
      //large REDUCE
      display.fillTriangle(displayWidth / 2 - 40, displayHeight / 2 - 30, displayWidth / 2 + 40, displayHeight / 2 - 30, displayWidth / 2, displayHeight / 2 + 30, GxEPD_BLACK);  // large down
    }
    // Be nice to draw white graph through triange? TBD


  } while (display.nextPage());
}

void writeDashedHLine(int16_t x0, int16_t y0, int16_t x1, int16_t dash_size) {
  // This writes a dashed horixontal line from x0 to x1 at height y0 with dash_size as px On then px Off
  int number_dashes = ((x1 - x0) / dash_size);
  for (int l = 0; l < number_dashes; l++) {

    if (l % 2 == 0) {
      // even - so draw line
      display.drawLine(x0 + (l * dash_size), y0, x0 + ((l + 1) * dash_size), y0, GxEPD_BLACK);
    }
  }
  display.drawLine(x0 + (number_dashes * dash_size), y0, x1, y0, GxEPD_BLACK);
}

void writeDashedVLine(int16_t y0, int16_t x0, int16_t y1, int16_t dash_size) {
  // This writes a dashed horixontal line from x0 to x1 at height y0 with dash_size as px On then px Off
  int number_dashes = ((y1 - y0) / dash_size);
  for (int l = 0; l < number_dashes; l++) {
    if (l % 2 == 0) {
      // even - so draw line
      display.drawLine(x0, y0 + (l * dash_size), x0, y0 + ((l + 1) * dash_size), GxEPD_BLACK);
    }
  }
  display.drawLine(x0, y0 + (number_dashes * dash_size), x0, y1, GxEPD_BLACK);
}
#endif


void displaySSID(String _AP_SSID, String _AP_PASS) {
  // Want to display the info here for people to log in:
  //displayInit();
  displayClear();
  display.setTextSize(1);
  display.setFullWindow();
  display.firstPage();
  do {
    displayText("Please connect to:", 15, CENTER_ALIGNMENT);
    displayText("192.168.4.1", 30, CENTER_ALIGNMENT);
    displayText("SSID:", 45, CENTER_ALIGNMENT);
    displayText(_AP_SSID, 60, CENTER_ALIGNMENT);
    displayText("PASSWORD:", 75, CENTER_ALIGNMENT);
    displayText(_AP_PASS, 90, CENTER_ALIGNMENT);
  } while (display.nextPage());
}

void displayShutDown() {
  // Tells the user that it has shut down - need to touch to restart in AP
  //displayInit();
  displayClear();
  display.setTextSize(1);
  display.setFullWindow();
  display.firstPage();
  do {
    displayText("WiFi / AP Problem", 30, CENTER_ALIGNMENT);
    displayText("Gone to sleep....", 60, CENTER_ALIGNMENT);
    displayText("Touch to wake up", 90, CENTER_ALIGNMENT);
  } while (display.nextPage());
}
