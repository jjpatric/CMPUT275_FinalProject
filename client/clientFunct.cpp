/*
 * Routine for drawing an image patch from the SD card to the LCD display.
 */

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <SPI.h>

#include "clientFunct.h"

void drawBuilding(char type, int pop, int textBackground, int xPos, int yPos, Adafruit_ILI9341 tft){
  tft.setCursor(xPos, yPos);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK, textBackground);
  tft.fillRect(xPos - 2, yPos - 2, 23, 10, textBackground);
  tft.print(type);
  tft.setCursor(xPos+8, yPos);
  tft.print(pop);
}

void drawMainMenu(Adafruit_ILI9341 tft){
  tft.fillRect(50, 50, 100, 75, 0);
}