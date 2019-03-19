#ifndef _CLIENT_FUNCT_H_
#define _CLIENT_FUNCT_H_

using namespace std;

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

struct xy_pos {
  int16_t x, y;
  xy_pos(int16_t x1 = 0, int16_t y1 = 0) : x(x1), y(y1) {}
};

void drawBuilding(char type, int pop, int textBackground, int xPos, int yPos);

void drawMainMenu();

#endif
