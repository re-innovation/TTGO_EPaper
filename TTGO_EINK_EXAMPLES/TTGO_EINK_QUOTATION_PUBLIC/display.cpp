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
void displayQuote(String _quote, String _author)
{
  // Need to check the size of the quote
  // If it is bigger than *** then need to chop into smaller strings and display parts
  // Display is 250px x 122px

  // This displays the quote on the e-ink display
  display.setTextSize(1);
  int _index = 0;
  String _local_string = _quote.substring(_index, _index + 22);
  int n = 10; // First line placement
  //while (_local_string.length() > 0)
  for (int y = 0; y < 5; y++)
  {
    // Max 6 lines of text available...
    // Find the final space in the string:
    _local_string = _quote.substring(0, 22);
    _index = 22;
    while (!isSpace(_local_string[_index]))
    {
      _index--;
    }
    displayText(_quote.substring(0, _index), n, CENTER_ALIGNMENT);
    n = n + 17; // Go to a new line
    int end_of_quote = _quote.length();
    _quote = _quote.substring(_index, end_of_quote);
    if (_quote.length() < 22)
    {
      displayText(_quote, n, CENTER_ALIGNMENT);
      break;
    }
  }
  displayText(_author, 116, CENTER_ALIGNMENT);
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
  //display.setTextSize(1);
  //display.setFont(&FreeMonoBold9pt7b);
  //display.update();
}


void displayUpdatingScreen()
{
  display.setCursor(0, 11);
  display.println("Finding");
  display.setCursor(0, 31);
  display.println("Quote...");
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
