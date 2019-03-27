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

shared_vars shared;

int main() {
  setup();

  shared.curr_mode = shared.STATE2;

  drawMainMenu();

  while (shared.curr_mode == shared.STATE2) {
    // clear entries for new state
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;

    process_input(); // read if buttons are pushed
    updateMainMenu(); // update Main Menu based on buttons pressed
    //read_line(); // read in anything coming from server

  }
  Serial.flush();
  return 0;
}
