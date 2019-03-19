//--------------------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project
//--------------------------------------------

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>
#include "clientFunct.h"


// max size of buffer, including null terminator
const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;

// pins for buttons
const uint8_t select_pin = 2;
const uint8_t action_pin = 3;

// pins for tft display
const uint8_t sd_cs = 6;
const uint8_t tft_cs = 10;
const uint8_t tft_dc = 9;

// analog pins for joystick axes
const uint8_t joy_x_pin = A1;
const uint8_t joy_y_pin = A0;

// joystick button pin
const uint8_t joy_button_pin = 8;

// screen dimensions
const int16_t tft_width = 320;
const int16_t tft_height = 240;

xy_pos joy_centre;

int backgroundColor = tft.color565(80, 130, 46);
int customRed = tft.color565(252, 70, 70);
int customBlue = tft.color565(79, 178, 255);

bool select_pushed, action_pushed, joy_button_pushed;

int cursorX = tft_width/2, cursorY = tft_height/2, cursorR = 9;

// input buffer
char* buffer = (char *) malloc(buf_size);

bool successHS = false;


void process_line() {
  
  if(buffer[0] == 65){ successHS = true; } //only checks first letter of handshake message
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

void setup() {
  // initialize Arduino
  init();

  // initialize zoom pins
  pinMode(select_pin, INPUT_PULLUP);
  pinMode(action_pin, INPUT_PULLUP);

  // initialize joystick pins and calibrate centre reading
  pinMode(joy_button_pin, INPUT_PULLUP);

  // x and y are reverse because of how our joystick is oriented
  joy_centre = xy_pos(analogRead(joy_y_pin), analogRead(joy_x_pin));

  // initialize serial port
  Serial.begin(9600);
  Serial.flush(); // get rid of any leftover bits

  // initialize display
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(backgroundColor);

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

  // testing layout:
  //drawBuilding('B', 10, customRed, 20, 20);
  //drawBuilding('B', 10, customRed, 20, 220);
  //drawBuilding('P', 25, ILI9341_WHITE, 160, 120);
  //drawBuilding('H', 50, customBlue, 260, 220);

}

int main() {
  setup();

  // very simple finite state machine:
  // which endpoint are we waiting for?
  enum {STATE2, STATE3} curr_mode = STATE2;

  drawMainMenu();

  while (curr_mode == STATE2) {
    // clear entries for new state
    select_pushed = 0;
    action_pushed = 0;
    joy_button_pushed = 0;


    //process_input();

  }
  Serial.flush();
  return 0;
}
