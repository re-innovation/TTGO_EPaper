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
  //display.eraseDisplay();
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
}

void displayRadiationInfo(int _dataNumber)
{

  // Here we want to display a different icon/text depending upon number.
  //_dataNumber = 10;   // debug testing here.
  float _uSv = _dataNumber * 0.0057;

  // Display the value on the right side of the screen.
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.println("CPM:");
  display.setTextSize(3);
  display.setCursor(40, 45);
  display.println(_dataNumber);

  display.setTextSize(1);
  display.setCursor(0, 70);
  display.println("uSv:");
  display.setTextSize(2);
  display.setCursor(0, 105);
  display.println(_uSv, 3);
  
  if (_dataNumber < RADIATION_LOW_CPM)
  {
    // LOW
    display.drawExampleBitmap(gImage_Radiation2_low_100x120px, 150, 0, 100, 120, GxEPD_BLACK);
  }
  else if (_dataNumber < RADIATION_MED_CPM)
  {
    // MEDIUM
    display.drawExampleBitmap(gImage_Radiation2_med_100x120px, 150, 0, 100, 120, GxEPD_BLACK);
  }
  else
  {
    // HIGH!!!!
    display.drawExampleBitmap(gImage_Radiation2_high_100x120px, 150, 0, 100, 120, GxEPD_BLACK);
    // Also highlight a warning!!
  }
}

void displayUpdatingScreen()
{
  display.setCursor(0, 11);
  display.println("Getting");
  display.setCursor(0, 31);
  display.println("Data...");
}

void displayUpdate()
{
  display.update();
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
  displayText("PASSWORD:", 75, CENTER_ALIGNMENT);
  displayText(_AP_PASS, 90, CENTER_ALIGNMENT);
  display.update();
}

void displayShutDown()
{
  // Tells the user that it has shut down - need to touch to restart in AP
  displayInit();
  displayClear();
  display.setTextSize(1);
  displayText("WiFi / AP Problem", 30, CENTER_ALIGNMENT);
  displayText("Gone to sleep....", 60, CENTER_ALIGNMENT);
  displayText("Touch to wake up", 90, CENTER_ALIGNMENT);
  display.update();
}

void displayNoMQTT()
{
  // Tells the user that there is an MQTT feed error
  displayInit();
  displayClear();
  display.setTextSize(1);
  displayText("Error", 30, CENTER_ALIGNMENT);
  displayText("Check MQTT Feed", 45, CENTER_ALIGNMENT);
  displayText("Gone to sleep....", 60, CENTER_ALIGNMENT);
  displayText("Touch to wake up", 90, CENTER_ALIGNMENT);
  display.update();
}
