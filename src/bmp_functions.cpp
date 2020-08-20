#include "bmp_functions.h"

void drawBmp(const char *filename, int16_t x, int16_t y, TFT_eSPI* tft) {

  if ((x >= tft->width()) || (y >= tft->height())) return;

  fs::File bmpFS;

  // Open requested file on SD card
  bmpFS = SPIFFS.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset, sec, third, fourth;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();
  uint16_t first = read16(bmpFS);
  //Serial.printf(String(first));
  Serial.print("first16 = ");
  Serial.println(first, HEX);
  if (first == 0x4D42)
  //if (read16(bmpFS) == 0x4D42)
  {
    sec = read32(bmpFS);
    Serial.print("sec32 = ");
    Serial.println(sec, HEX);
    third = read32(bmpFS);
    Serial.print("third32 = ");
    Serial.println(third, HEX);
    seekOffset = read32(bmpFS);
    Serial.print("offset32 = ");
    Serial.println(seekOffset, HEX);
    fourth = read32(bmpFS);
    Serial.print("fourth32 = ");
    Serial.println(fourth, HEX);
    w = read32(bmpFS);
    Serial.println(w);
    h = read32(bmpFS);
    Serial.println(h);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      bool oldSwapBytes = tft->getSwapBytes();
      tft->setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16 bit colours
        for (col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        tft->pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      tft->setSwapBytes(oldSwapBytes);
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  //Serial.write(&result[0]);
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}