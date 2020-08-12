#include "settings.h"
#include "TaskScheduler.h"
#include "sensor_readings.h"
#include "network_config.h"
#include "bmp_functions.h"
#include "clock.h"
#include "aux_functions.h"

void sensor_readings_update();
void clock_update();

// bme is global to this file only
Adafruit_BME280 bme;
// tft is global to this file only
TFT_eSPI tft = TFT_eSPI();
// Setup the clock 
Timezone edmontonTZ;

uint16_t bg = TFT_BLACK;
uint16_t fg = TFT_WHITE;

// Setup tasks for the task scheduler
// The third argument takes a pointer to a function, but cannot have parameters.
Task t1_bme280(10000, TASK_FOREVER, &sensor_readings_update);
Task t2_clock(1000, TASK_FOREVER, &clock_update);

// Create the scheduler
Scheduler runner;

// Adafruit.IO feeds
// Avoid underscores in the feed names, they cause problems with groupings.
AdafruitIO_Feed *temperature    = io.feed("lab-environment.temperature");
AdafruitIO_Feed *humidity       = io.feed("lab-environment.humidity");
AdafruitIO_Feed *barpressure    = io.feed("lab-environment.barpressure");
AdafruitIO_Feed *altitude       = io.feed("lab-environment.altitude");

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

// Wifi Setup

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
  // Connect to Wifi
  wifiStatus(&tft, &io);
  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
      Serial.print(".");
  delay(500);
    }
  // Check the Wifi status
  wifiStatus(&tft, &io);
  delay(2000);

  // Setup the clock
  waitForSync();

  edmontonTZ.setLocation("America/Edmonton");

  // Start the task scheduler
  runner.init();
  // Add the task to the scheduler
  runner.addTask(t1_bme280);
  runner.addTask(t2_clock);
  // Enable the task
  t1_bme280.enable();
  t2_clock.enable();

  tft.fillScreen(bg);
  drawBmp("/te.bmp", 160, 198, &tft);
}

void loop() {
  // Execute the scheduler runner
  runner.execute();
  // Update the MQTT queue and stay connected to Adafruit IO
  io.run();
  // Update the clock
  //events();
}

void sensor_readings_update()
{
  refresh_readings_bme280( &bme, 
                    &tft, 
                    temperature, 
                    humidity, 
                    barpressure, 
                    altitude);
}

void clock_update()
{
  refresh_clock(&tft, &edmontonTZ);
}