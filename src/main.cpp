#include <Arduino.h>
#include "sensor_readings.h"
#include "TFT_eSPI.h"     // ESP32 Hardware-specific library
#include "settings.h"    // The order is important!
#include "bmp_functions.h"
#include "TaskScheduler.h"

void sensor_readings_update();

// bme is global to this file only
Adafruit_BME280 bme;
// tft is global to this file only
TFT_eSPI tft = TFT_eSPI();

uint16_t bg = TFT_BLACK;
uint16_t fg = TFT_WHITE;

// Setup tasks for the task scheduler
// The third argument taks a pointer to a function, but cannot have parameters.
Task t1_bme280(2000, TASK_FOREVER, &sensor_readings_update); 

// Create the scheduler
Scheduler runner;

void initSPIFFS()
{
    if (!SPIFFS.begin())
    {
        Serial.println("Cannot mount SPIFFS volume...");
        while (1){}
    }
    else
    {
        Serial.println("SPIFFS volume mounted properly");
    }
}

void setup() {
  Serial.begin(9600);
  initSPIFFS();
  bool status;
  // Setup the TFT
  tft.begin();
  tft.setRotation(3);
  tft.loadFont("NotoSansBold20");
  tft.setTextColor(fg, bg);
  tft.fillScreen(bg);
  tft.setCursor(0, 0);
  tft.println("Hello!");
  tft.println("Starting BME sensor...");
  delay(5000);

  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);  // Infinite loop
  }

  // Start the task scheduler
  runner.init();

  // Add the task to the scheduler
  runner.addTask(t1_bme280);

  // Enable the task
  t1_bme280.enable();

  tft.fillScreen(bg);
  drawBmp("/blueegg.bmp", 160, 180, &tft);
}

void loop() {
  // Execute the scheduler runner
  runner.execute();
}

void sensor_readings_update()
{
  refresh_readings_bme280(&bme, &tft);
}