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
#include "game.h"

shared_vars shared; // idea from eClass notes stores shared variables in "clientFunct.h"

int main() {
  setup();

  shared.curr_mode = shared.STATE2;

  drawMainMenu();

  char modeSelect, mapSelect;

  while (shared.curr_mode == shared.STATE2) {
    // clear entries for new state (Idea from eClass)
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
    // clear entries for new state (Idea from eClass)
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;

    process_input(); // read if buttons are pushed
    mapSelect = updateMapMenu();

    Serial.println(mapSelect);
    Serial.flush();

    read_line();

  }

  int turn = 1;
  while(shared.curr_mode == shared.STATE4){
    updateGame();

    playerTurn(turn);

    sendData();

    if(turn == 1) turn = 2; // switch whos turn it is
    else turn = 1;

  }
  return 0;
}
