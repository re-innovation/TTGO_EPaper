#include "display.h"
#include "utilities.h"

// For Eink Paper - Use gxEPD Library
// include library, include base class, make path known
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include "CE_Icons.h"        // Curious Electric Icons

GxIO_Class io(SPI, ELINK_SS, ELINK_DC, ELINK_RESET);
GxEPD_Class display(io, ELINK_RESET, ELINK_BUSY);

void displayTest()
{
  for (int n = 0; n < 20; n++)
  {
    Serial.println("TEST");
  }
}

//#########################################################################
void displayShowCELogo()
{
  // This routine shows the Curious Electric Logo on the screen. Always Useful!!
  display.drawExampleBitmap(gImage_CE_Logo_test_data_100x120, 140, 0, 100, 120, GxEPD_BLACK);
}
//#########################################################################
void displayInit()
{
  static bool isInit = false;
  if (isInit) {
    return;
  }
  isInit = true;    // This only lets the init run once...
  display.init();
  display.setRotation(1);
  display.eraseDisplay();
  display.setTextColor(GxEPD_BLACK);
  //  display.setFont(&DEFALUT_FONT);
  //  display.setTextSize(0);
  display.fillScreen(GxEPD_WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoBold9pt7b);
  //display.update();
}

void displayClear()
{
  display.fillScreen(GxEPD_WHITE);
  //display.setTextSize(1);
  //display.setFont(&FreeMonoBold9pt7b);
  //display.update();
}

void displayRadiationInfo(int _dataNumber)
{
  display.setCursor(0, 11);
  display.println("CPM:");
  display.setCursor(50, 11);
  display.println(_dataNumber);
}

void displayUpdatingScreen(String _display_string)
{
  display.setCursor(0, 11);
  display.println("Finding...");
  display.setCursor(0, 31);
  display.println(_display_string);
}

void displayUpdate()
{
  display.update();
}

//#########################################################################################

void displayCOVIDInfo(COVID_record_type covid_data[])
{
  // 2.13" e-paper display is 250 x 122 px resolution

  // Display:
  // Location   Date
  // New Cases, Cumulative case
  // New Deaths, Cumulative Deaths
  // Graphic for change
  // Percentage change from previous day/7 day average?

  display.setTextSize(0);
  display.setCursor(0, 11);
  display.print(covid_data[0].areaName);
  display.setCursor(0, 28);
  display.print(covid_data[0].date);

  // Here want to display the time and date of most recent update:
  display.setTextSize(1);
  display.setCursor(0, 105);
  display.print(covid_data[0].newCases);
  display.setTextSize(0);
  display.print(" of ");
  display.print(covid_data[0].totalCases);

  display.setTextSize(1);
  display.setCursor(0, 121);
  display.print(covid_data[0].newDeaths);
  display.setTextSize(0);
  display.print(" of ");
  display.print(covid_data[0].totalDeaths);
  //display.update();
}

void displayCOVIDGraph(COVID_record_type my_covid_data[], int _x, int _y, int _height, int _width)
{
  // This shows a graph of the data within the covid_data[] array
  // It is drawn at the starting point _x and _y

  display.drawLine(_x, _y, (_width + _x), _y, GxEPD_BLACK);

  // This line is an example of how to do a rectangle
  //display.fillRect(_x, _y, _width, -_height, GxEPD_BLACK);

  int width_section = _width / (DATA_STORED);

  // Want to find the maximum from the data available:
  int maximum_value = 0;
  for (int y = 0; y < DATA_STORED; y++)
  {
    if (my_covid_data[y].newCases > maximum_value)
    {
      maximum_value = my_covid_data[y].newCases;
    }
  }
  Serial.print("Maximum: ");
  Serial.println(maximum_value);

  int height_section;
  for (int i = 0; i < DATA_STORED; i++)
  {
    height_section = ((my_covid_data[i].newCases) * _height) / maximum_value;
    display.fillRect((_x + _width - ((i + 1) * width_section)), _y, width_section, -height_section, GxEPD_BLACK);
  }
}

//#################################################################################
void displayText(const String &_str, int16_t _y, uint8_t _alignment)
{
  int16_t x = 0;
  int16_t x1, y1;
  uint16_t w, h;
  display.setCursor(x, _y);
  display.getTextBounds(_str, x, _y, &x1, &y1, &w, &h);

  switch (_alignment) {
    case RIGHT_ALIGNMENT:
      display.setCursor(display.width() - w - x1, _y);
      break;
    case LEFT_ALIGNMENT:
      display.setCursor(0, _y);
      break;
    case CENTER_ALIGNMENT:
      display.setCursor(display.width() / 2 - ((w + x1) / 2), _y);
      break;
    default:
      break;
  }
  display.println(_str);
}


//#########################################################################################
void displayCEIcon(int x, int y, String IconName) {

  // TTGO display is: 250px x 122px
  // Icon needs to be inverted to use here???
  uint16_t invert = GxEPD::bm_invert;
  //display.drawRect(x, y, 130, 130, GxEPD_BLACK);
  // NOTE: Using 'drawInvertedBitmap' and not 'drawBitmap' so that images are WYSIWYG, otherwise all images need to be inverted

  if      (IconName == "01d" || IconName == "01n")  display.drawExampleBitmap(gImage_sunny,       x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "02d" || IconName == "02n")  display.drawExampleBitmap(gImage_mostlySunny, x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "03d" || IconName == "03n")  display.drawExampleBitmap(gImage_cloudy,      x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "04d" || IconName == "04n")  display.drawExampleBitmap(gImage_mostlySunny, x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "09d" || IconName == "09n")  display.drawExampleBitmap(gImage_chanceRain,  x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "10d" || IconName == "10n")  display.drawExampleBitmap(gImage_rain,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "11d" || IconName == "11n")  display.drawExampleBitmap(gImage_lightning,   x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "13d" || IconName == "13n")  display.drawExampleBitmap(gImage_snow,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "50d")                       display.drawExampleBitmap(gImage_haze,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "50n")                       display.drawExampleBitmap(gImage_fog,         x, y, 50, 50, GxEPD_BLACK);
  else                                              display.drawExampleBitmap(gImage_noData,      x, y, 50, 50, GxEPD_BLACK);

}

void displaySSID(String _AP_SSID, String _AP_PASS)
{
  // Want to display the info here for people to log in:
  displayInit();
  displayClear();
  display.setTextSize(1);
  displayText("Please connect to:", 15, CENTER_ALIGNMENT);
  displayText("198.168.4.1", 30, CENTER_ALIGNMENT);
  displayText("SSID:", 45, CENTER_ALIGNMENT);
  displayText(_AP_SSID, 60, CENTER_ALIGNMENT);
  displayText("PASS:", 75, CENTER_ALIGNMENT);
  displayText(_AP_PASS, 90, CENTER_ALIGNMENT);
  display.update();
}
