#include <Arduino.h>
#include "sensor_readings.h"
#include "SPIFFS.h"
//#include "TFT_eSPI.h"     // ESP32 Hardware-specific library
#include "settings.h"    // The order is important!
#include "bmp_functions.h"
#include "TaskScheduler.h"
#include "WiFi.h"
//#include <ESPAsyncWebServer.h>
#include "network_config.h"

void sensor_readings_update();
void clock_update();


// WiFi credentials
char WIFI_SSID_STRING[20] = "OldRob";
char WIFI_PASS_STRING[20] = "5223qaz7542PLM";
// bme is global to this file only
Adafruit_BME280 bme;
// tft is global to this file only
TFT_eSPI tft = TFT_eSPI();
// Setup the clock 
Timezone sydneyTZ;

uint16_t bg = TFT_BLACK;
uint16_t fg = TFT_WHITE;

// Setup tasks for the task scheduler
// The third argument taks a pointer to a function, but cannot have parameters.
Task t1_bme280(2000, TASK_FOREVER, &sensor_readings_update);
Task t2_clock(1000, TASK_FOREVER, &clock_update);

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

// Wifi Setup
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID_STRING, WIFI_PASS_STRING);
    Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(9600);
  initSPIFFS();
  //initWiFi();
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
  io.connect();

  // Check the Wifi status
  // wifiStatus();

  // Setup the clock
  waitForSync();

  sydneyTZ.setLocation("Australia/Sydney");

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
  // Update the clock
  //events();
}

void sensor_readings_update()
{
  refresh_readings_bme280(&bme, &tft);
}

void clock_update()
{
  refresh_clock(&tft, &sydneyTZ);
}