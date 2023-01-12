#include <Arduino.h>
#include "settings.h"
#include "clock.h"

void refresh_clock(TFT_eSPI* tft, Timezone* timezone)
{
  //See documentation for ezTime options: https://github.com/ropg/ezTime
  tft->loadFont("NotoSansBold20");
  // See available colors at https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_eSPI.h
  tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);  
  //Clear the date area
  tft->fillRect(200, 50, 180, 70, TFT_BLACK);
  // Day of week, day of month
  tft->setCursor(200, 50);
  tft->print(timezone->dateTime("l")); // Day of week
  // Month and day of month
  tft->setCursor(200, 65);
  tft->print(timezone->dateTime("F")); // Month
  tft->print(" ");
  tft->print(timezone->dateTime("j"));  // Day of month
  // Year
  tft->setCursor(200, 80);
  tft->print(timezone->dateTime("Y")); // Year
  // Time
  tft->setCursor(200, 95);
  tft->print(timezone->dateTime("H:i:s")); // Time

//   indicators();
}