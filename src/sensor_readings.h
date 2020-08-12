#include "settings.h"
#ifndef SENSOR_READINGS_H
    #define SENSOR_READINGS_H
    
    #define SEALEVELPRESSURE_HPA (1013.25)

    void refresh_readings_bme280(  Adafruit_BME280* bme, 
                            TFT_eSPI* tft,
                            AdafruitIO_Feed* temp,
                            AdafruitIO_Feed* hum,
                            AdafruitIO_Feed* bar,
                            AdafruitIO_Feed* alt);

#endif

