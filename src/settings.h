#ifndef SETTINGS_H
    #define SETTINGS_H
    #define LED_BUILTIN 2   // This is valid for my devkit
    #include <TFT_eSPI.h>           // ESP32 Hardware-specific library
    //#include "ezTime.h"             // The time and date library
    //#include "aux_functions.h"      // Helpful functions
    //#include "clock.h"
    // uint16_t bg = TFT_BLACK;
    // uint16_t fg = TFT_WHITE;
    #define LED_PIN            17
    #define IFTTT_PIN          16
#endif

#ifdef DEBUGLOG
#define DEBUGPRINT(x)     Serial.print (x)
#define DEBUGPRINTDEC(x)     Serial.print (x, DEC)
#define DEBUGPRINTLN(x)  Serial.println (x)
#else
#define DEBUGPRINT(x)
#define DEBUGPRINTDEC(x)
#define DEBUGPRINTLN(x)
#endif