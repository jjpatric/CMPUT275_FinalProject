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

  char modeSelect, mapSelect;

  while (shared.curr_mode == shared.STATE2) {
    // clear entries for new state
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;

    process_input(); // read if buttons are pushed
    modeSelect = updateMainMenu(); // update Main Menu based on buttons pressed

    Serial.println(modeSelect);
    Serial.flush();
    delay(100);
    read_line();

  }

  drawMapMenu();

  while(shared.curr_mode == shared.STATE3){
    // clear entries for new state
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;

    process_input(); // read if buttons are pushed
    mapSelect = updateMapMenu();

    Serial.println(mapSelect);
    Serial.flush();

    read_line();

  }


  while(shared.curr_mode == shared.STATE4){
    updateGame();

    return 0;

  }
  return 0;
}
