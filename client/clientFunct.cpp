//--------------------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project: Civilization Wars
//--------------------------------------------

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <SPI.h>

#include "clientFunct.h"
#include "game.h"

extern shared_vars shared;

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

// pins for buttons
const uint8_t select_pin = 2;
const uint8_t action_pin = 3;
const uint8_t move_pin = 8;

// pins for tft display
const uint8_t sd_cs = 6;
const uint8_t tft_cs = 10;
const uint8_t tft_dc = 9;

// screen dimensions
const int16_t tft_width = 320;
const int16_t tft_height = 240;

// max size of buffer, including null terminator
const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;
// input buffer
char* buffer = (char *) malloc(buf_size);

// custom game colours
int backgroundColor = tft.color565(80, 130, 46);
int customRed = tft.color565(252, 70, 70);
int customBlue = tft.color565(79, 178, 255);
int customYell = tft.color565(255, 242, 0);
int customWhite = tft.color565(255, 255, 255);

bool successHS = false;
bool mainSelect = true; // says if multiplayer is selected
int mapNum = 1;


// variables used in drawMainMenu() and updateMainMenu()
int multiplayerX = 55, multiplayerY = 100, multiplayerWidth = 200, multiplayerHeight = 50;

void drawMainMenu(){
  mainSelect = true;

  tft.fillScreen(backgroundColor);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setCursor(60,30);
  tft.setTextColor(customYell);
  tft.setTextSize(1);
  tft.print("Civilization");
  tft.setCursor(90,50);
  tft.print("Wars");

  tft.fillRect(multiplayerX, multiplayerY, multiplayerWidth, multiplayerHeight, customRed);
  tft.setCursor(multiplayerX + 20, multiplayerY + 25);
  tft.setTextColor(customBlue);
  tft.print("Multiplayer");
}

char updateMainMenu(){
  if(mainSelect){
    tft.drawRect(multiplayerX, multiplayerY, multiplayerWidth,
     multiplayerHeight, customYell); // select multiplayer
  }

  if(shared.action_pushed){
    return 'M';
  }

  return 'N';
}


// variables used in drawMapMenu() and updateMapMenu()
int mapOneX = 30, mapOneY = 50, mapOneWidth = 110, mapOneHeight = 60;
int mapTwoX = 190, mapTwoY = 50, mapTwoWidth = 110, mapTwoHeight = 60;
int mapThreeX = 30, mapThreeY = 140, mapThreeWidth = 110, mapThreeHeight = 60;
int mapCustX = 190, mapCustY = 140, mapCustWidth = 110, mapCustHeight = 60;

void drawMapMenu(){

  tft.fillScreen(backgroundColor);
  tft.setFont(&FreeMonoBoldOblique12pt7b);

  tft.fillRect(mapOneX, mapOneY, mapOneWidth, mapOneHeight, customRed);
  tft.setCursor(mapOneX + 45, mapOneY + 34);
  tft.setTextColor(customBlue);
  tft.print("1");

  tft.fillRect(mapTwoX, mapTwoY, mapTwoWidth, mapTwoHeight, customRed);
  tft.setCursor(mapTwoX + 45, mapTwoY + 34);
  tft.print("2");

  tft.fillRect(mapThreeX, mapThreeY, mapThreeWidth, mapThreeHeight, customRed);
  tft.setCursor(mapThreeX + 45, mapThreeY + 34);
  tft.print("3");

  tft.fillRect(mapCustX, mapCustY, mapCustWidth, mapCustHeight, customRed);
  tft.setCursor(mapCustX + 10, mapCustY + 34);
  tft.print("CUSTOM");

}

char updateMapMenu(){

  if(mapNum == 1){
    tft.drawRect(mapOneX, mapOneY, mapOneWidth,
     mapOneHeight, customYell); // select mapOne
    tft.drawRect(mapCustX, mapCustY, mapCustWidth,
     mapCustHeight, backgroundColor); // un-select Custom map
  }
  else if(mapNum == 2){
    tft.drawRect(mapTwoX, mapTwoY, mapTwoWidth,
     mapTwoHeight, customYell); // select mapTwp
    tft.drawRect(mapOneX, mapOneY, mapOneWidth,
     mapOneHeight, backgroundColor); // un-select mapOne
  }
  else if(mapNum == 3){
    tft.drawRect(mapThreeX, mapThreeY, mapThreeWidth,
     mapThreeHeight, customYell); // select mapThree
    tft.drawRect(mapTwoX, mapTwoY, mapTwoWidth,
     mapTwoHeight, backgroundColor); // un-select mapTwo
  }
  else if(mapNum == 4){
    tft.drawRect(mapCustX, mapCustY, mapCustWidth,
     mapCustHeight, customYell); // select custom Map
    tft.drawRect(mapThreeX, mapThreeY, mapThreeWidth,
     mapThreeHeight, backgroundColor); // un-select map Three
  }

  if(shared.action_pushed){
    if(mapNum == 1){
      return '1';
    }
    else if(mapNum == 2){
      return '2';
    }
    else if(mapNum == 3){
      return '3';
    }
    else if(mapNum == 4){
      return '4';
    }
  }

  return 'N';
}


void process_line() {
  
  if(buffer[0] == 65){ successHS = true; } //checks first letter of message is 'A'
  if(buffer[0] == 66){ makeBuilding(); } // if first character is 'B' make buildings
  if(buffer[0] == 67){ // if first character is 'C', then mode was recieved by server
    shared.curr_mode = shared.STATE3;
  }
  if(buffer[0] == 68){ // if first character is 'D', then map was recieved by server
    tft.fillScreen(backgroundColor);
    shared.curr_mode = shared.STATE4;
  }
  if(buffer[0] == 69){ // if first character is 'E', then stop reading buildings or Units
    shared.readBuildings = false;
    shared.readUnits = false;
  }
  if(buffer[0] == 70){ // if first character is 'F', then data was recieved by server
    shared.readData = false;
  }
  if(buffer[0] == 85){ // if first character is 'U', then make Units
    makeUnit();
  }

  // clear the buffer
  buf_len = 0;
  buffer[buf_len] = 0;
}


void read_line() {
  char in_char;

  if (Serial.available()) {
    // read the incoming byte:
    char in_char = Serial.read();

    // if end of line is received, waiting for line is done:
    if (in_char == '\n' || in_char == '\r') {
      // now we process the buffer
      process_line();

      }
    else {
      // add character to buffer, provided that we don't overflow.
      // drop any excess characters.
      if ( buf_len < buf_size-1 ) {
        buffer[buf_len] = in_char;
        buf_len++;
        buffer[buf_len] = 0;
      }
    }
  }
}


void process_input(){
  shared.action_pushed = (digitalRead(action_pin) == LOW);
  if(shared.action_pushed) {
    while(digitalRead(action_pin) == LOW){} // wait until button is released
    delay(120); // prevent double clicks
  while(digitalRead(action_pin) == LOW){} // wait until button is released
  }
  shared.select_pushed = (digitalRead(select_pin) == LOW);
  if(shared.select_pushed) {
    while(digitalRead(select_pin) == LOW){} // wait until button is released
    delay(120); // prevent double clicks
    while(digitalRead(select_pin) == LOW){} // wait until button is released
  }
  shared.move_pushed = (digitalRead(move_pin) == LOW);
  if(shared.move_pushed) {
    while(digitalRead(move_pin) == LOW){} // wait until button is released
    delay(120); // prevent double clicks
    while(digitalRead(move_pin) == LOW){} // wait until button is released
  }
  
  if(shared.move_pushed && shared.curr_mode == shared.STATE2) mainSelect = !mainSelect; // if in main menu switch which thing is selected
  if(shared.move_pushed && shared.curr_mode == shared.STATE3){
    mapNum++; // if in map menu, switch which thing is selected
    if(mapNum >= 5){ mapNum = 1; }
  }
}


void setup() {
  // initialize Arduino
  init();

  // initialize button pins
  pinMode(select_pin, INPUT_PULLUP);
  pinMode(action_pin, INPUT_PULLUP);
  pinMode(move_pin, INPUT_PULLUP);

  // initialize serial port
  Serial.begin(9600);
  Serial.flush(); // get rid of any leftover bits

  // initialize display
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(backgroundColor);
  tft.println("Connect to server...");

  // set up buffer as empty string
  buf_len = 0;
  buffer[buf_len] = 0;


  while(!successHS){
    Serial.println("STATE1");
    Serial.flush();
    delay(501); // time for server to respond
    read_line();
  }
  Serial.println("STATE2");
  Serial.flush();

}