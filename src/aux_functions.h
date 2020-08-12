#include <Arduino.h>
#include "settings.h"

void wifiStatus(    TFT_eSPI* tft, 
                    AdafruitIO_WiFi* io);

void indicators(    TFT_eSPI* tft, 
                    AdafruitIO_WiFi* io);

const char* wl_status_to_string(wl_status_t status);