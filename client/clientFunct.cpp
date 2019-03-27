/*
 * Routine for drawing an image patch from the SD card to the LCD display.
 */

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <SPI.h>

#include "clientFunct.h"

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

int backgroundColor = tft.color565(80, 130, 46);
int customRed = tft.color565(252, 70, 70);
int customBlue = tft.color565(79, 178, 255);

bool successHS = false;
bool mainSelect = true; // says if multiplayer is selected


void drawBuilding(char type, int pop, int textBackground, int xPos, int yPos){
  tft.setFont();
  tft.setCursor(xPos, yPos);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK, textBackground);
  tft.fillRect(xPos - 2, yPos - 2, 23, 10, textBackground);
  tft.print(type);
  tft.setCursor(xPos+8, yPos);
  tft.print(pop);
}


void makeBuilding(){
  int i = 0, count = 0;
  char type;
  int pop, team, textBackground, xPos, yPos;

  while(true){
    if(buffer[i] == 32 && count == 0){
      type = buffer[i+1];
      count++;
    }
    else if(buffer[i] == 32 && count == 1){
      pop = atoi(&buffer[i+1]);
      count++;
    }
    else if(buffer[i] == 32 && count == 2){
      team = atoi(&buffer[i+1]);
      textBackground;
      if (team == 0){ textBackground = ILI9341_WHITE; }
      else if (team == 1){ textBackground = customRed; }
      else if (team == 2){ textBackground = customBlue; }
      count++;
    }
    else if(buffer[i] == 32 && count == 3){
      xPos = atoi(&buffer[i+1]);
      count++;
    }
    else if(buffer[i] == 32 && count == 4){
      yPos = atoi(&buffer[i+1]);
      count++;

    }
    else if(buffer[i] == 10 && count == 5){
      break;
    }
    i++;
  }


  drawBuilding(type, pop, textBackground, xPos, yPos);


  Serial.print("A\n");
  Serial.flush();
 
}

int multiplayerCornerX = 55, multiplayerCornerY = 100, multiplayerWidth = 200, multiplayerHeight = 50;
int computerCornerX = 55, computerCornerY = 175, computerWidth = 200, computerHeight = 50;

void drawMainMenu(){
  mainSelect = true;

  tft.fillScreen(backgroundColor);
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setCursor(60,30);
  tft.setTextColor(tft.color565(255, 242, 0));
  tft.setTextSize(1);
  tft.print("Civilization");
  tft.setCursor(90,50);
  tft.print("Wars");

  tft.fillRect(multiplayerCornerX, multiplayerCornerY, multiplayerWidth, multiplayerHeight, customRed);
  tft.setCursor(multiplayerCornerX + 20, multiplayerCornerY + 25);
  tft.setTextColor(customBlue);
  tft.print("Multiplayer");

  tft.fillRect(computerCornerX, computerCornerY, computerWidth, computerHeight, customBlue);
  tft.setCursor(computerCornerX + 20, computerCornerY + 25);
  tft.setTextColor(customRed);
  tft.print("Computer");

}

void updateMainMenu(){

    if(mainSelect){
        tft.drawRect(multiplayerCornerX, multiplayerCornerY, multiplayerWidth,
         multiplayerHeight, tft.color565(255, 242, 0)); // select multiplayer
        tft.drawRect(computerCornerX, computerCornerY, computerWidth,
         computerHeight, backgroundColor); // un-select computer
    }
    else{
        tft.drawRect(computerCornerX, computerCornerY, computerWidth,
         computerHeight, tft.color565(255, 242, 0)); // select computer
        tft.drawRect(multiplayerCornerX, multiplayerCornerY, multiplayerWidth,
         multiplayerHeight, backgroundColor); // un-select multiplayer
    }
}


void process_line() {
  
  if(buffer[0] == 65){ successHS = true; } //only checks first letter of handshake message
  if(buffer[0] == 66){ makeBuilding(); } // if first character is 'B'

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
    }
    shared.select_pushed = (digitalRead(select_pin) == LOW);
    if(shared.select_pushed) {
        while(digitalRead(select_pin) == LOW){} // wait until button is released
    }
    shared.move_pushed = (digitalRead(move_pin) == LOW);
    if(shared.move_pushed) {
        while(digitalRead(move_pin) == LOW){} // wait until button is released
    }
    
    if(shared.move_pushed && shared.curr_mode == shared.STATE2) mainSelect = !mainSelect; // if in main menu switch which thing is selected
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
    delay(100);
    read_line();
  }
  Serial.println("STATE2");
  Serial.flush();

}