#include <Arduino.h>
#include "aux_functions.h"

// uint16_t bg = TFT_BLACK;
// uint16_t fg = TFT_WHITE;

void wifiStatus(  TFT_eSPI* tft, 
                  AdafruitIO_WiFi* io)
{
  // Wifi status
  tft -> loadFont("NotoSansBold20");
  tft -> setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft -> fillRect(5, 220, 150, 20, TFT_BLACK);
  tft -> setCursor(5, 220);
  tft -> print(wl_status_to_string(WiFi.status()));
  //tft -> print(io -> networkStatus());
}

void indicators(  TFT_eSPI* tft, 
                  AdafruitIO_WiFi* io)
{
  wifiStatus(tft, io);
}

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}