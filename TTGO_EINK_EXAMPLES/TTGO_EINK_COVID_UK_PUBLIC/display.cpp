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

void displayUpdatingScreen()
{
  display.setCursor(0, 11);
  display.println("Finding...");
  display.setCursor(0, 31);
  display.println(MY_CITY);
  //  display.setCursor(0, 51);
  //  display.println(MY_COUNTRY);
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
  // Location: 
  // New Cases, Cumulative case
  // New Deaths, Cumulative Deaths
  // Graphic for change
  // Percentage change from previous day/7 day average?

  // Here want to display the time and date of most recent update:
  display.setTextSize(0);
  display.setCursor(0, 21);
  display.println("Cases:");
  display.setCursor(0, 51);
  display.println("Total:");
  display.setCursor(0, 81);
  display.println("Deaths:");
  display.setCursor(0, 111);
  display.println("Total:");

  display.setTextSize(2);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(80, 21);
  display.println(covid_data[0].newCasesBySpecimenDate);
  display.setCursor(80, 81);
  display.println(covid_data[0].newDeathsByDeathDate);

  display.setTextSize(0);  
  display.setCursor(80, 51);
  display.println(covid_data[0].cumCasesBySpecimenDate);
  display.setCursor(80, 111);
  display.println(covid_data[0].cumDeathsByDeathDate);

  display.setTextSize(0);
  display.setCursor(180, 21);
  display.println("Daily");
  display.setCursor(180, 81);
  display.println("Daily");
  
  //display.update();
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
  display.setTextSize(1);
  displayText("Please connect to:", 15, CENTER_ALIGNMENT);
  displayText("198.168.4.1", 30, CENTER_ALIGNMENT);
  displayText("SSID:", 45, CENTER_ALIGNMENT);
  displayText(_AP_SSID, 60, CENTER_ALIGNMENT);
  displayText("PASS:", 75, CENTER_ALIGNMENT);
  displayText(_AP_PASS, 90, CENTER_ALIGNMENT);
  display.update();
}
