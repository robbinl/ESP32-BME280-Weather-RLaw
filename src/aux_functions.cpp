#include <Arduino.h>
#include "settings.h"
#include "aux_functions.h"

int postCounter = 0; // Use this counter to indicate (on the TFT screen) how many
                     // MQTT posts where completed in the current power cycle

void wifiStatus(  TFT_eSPI* tft, 
                  AdafruitIO_WiFi* io)
{
  tft -> loadFont("NotoSansBold20");
  tft -> setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft -> fillRect(5, 220, 150, 20, TFT_BLACK);
  tft -> setCursor(5, 220);
  tft -> print(wl_status_to_string(WiFi.status()));
}

void indicators(  TFT_eSPI* tft, 
                  AdafruitIO_WiFi* io)
{
  wifiStatus(tft, io);
}

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "NO_SHIELD";
    case WL_IDLE_STATUS: return "IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
    case WL_CONNECTED: return "CONNECTED";
    case WL_CONNECT_FAILED: return "CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION_LOST";
    case WL_DISCONNECTED: return "DISCONNECTED";
    default: return "";
  }
}

void postsCounter(  TFT_eSPI* tft)
{
  //int maxPosts = EEPROM.readInt(0);
  int maxPosts = EEPROM.readInt(0);
  Serial.println(maxPosts);
  postCounter++; // increment the postCounter by one.
  Serial.println(postCounter);
  //if (postCounter > maxPosts)
  //{
    maxPosts++;
    //Serial.println(maxPosts);
    //EEPROM.writeInt(0, maxPosts); // Write the new maxPosts in the EEPROM
    EEPROM.writeInt(0, maxPosts);
    EEPROM.commit();
  //}

  tft->loadFont("NotoSansBold20");
  tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft->fillRect(220, 0, 90, 20, TFT_BLACK);
  tft->setCursor(220, 0);
  tft->print(postCounter);
  tft->print("/");
  tft->print(EEPROM.readInt(0));
}