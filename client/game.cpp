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
#include <SPI.h>

#include "clientFunct.h"
#include "game.h"

extern shared_vars shared;
extern char* buffer;

extern int backgroundColor;
extern int customRed;
extern int customBlue;
extern int customYell;
extern int customWhite;

extern Adafruit_ILI9341 tft;

buildings town[30]; // max number of buildings is 30
units armys[100]; // unlikely there will be over 100 armys
int moveToBuild;
int numBuilds = 0;
int numArmys = 0;

void drawBuilding(char type, int pop, int teamColor, int xPos, int yPos){
  tft.setFont(); // reset font
  tft.setCursor(xPos, yPos); // reset other text options
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK, teamColor);
  tft.fillRect(xPos - 2, yPos - 2, 23, 10, teamColor); // draw building
  tft.print(type);
  tft.setCursor(xPos+8, yPos);
  tft.print(pop);
}


void makeBuilding(){
  int i = 0, count = 0;
  char type;
  int pop, team, teamColor, xPos, yPos;

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
      if (team == 0){ teamColor = ILI9341_WHITE; } // neutral
      else if (team == 1){ teamColor = customRed; } // team 1
      else if (team == 2){ teamColor = customBlue; } // team 2
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

  town[numBuilds].x = xPos;
  town[numBuilds].y = yPos;
  town[numBuilds].team = team;
  numBuilds++;
  drawBuilding(type, pop, teamColor, xPos, yPos);


  Serial.print("A\n");
  Serial.flush();
}


void makeUnit(){
  int i = 0, count = 0;
  int strength, team, teamColor, xPos, yPos;

  while(true){
    if(buffer[i] == 32 && count == 0){
      strength = atoi(&buffer[i+1]);
      count++;
    }
    else if(buffer[i] == 32 && count == 1){
      team = atoi(&buffer[i+1]);
      if (team == 1){ teamColor = customRed; } // team 1
      else if (team == 2){ teamColor = customBlue; } // team 2
      count++;
    }
    else if(buffer[i] == 32 && count == 2){
      xPos = atoi(&buffer[i+1]);
      count++;
    }
    else if(buffer[i] == 32 && count == 3){
      yPos = atoi(&buffer[i+1]);
      count++;
    }
    else if(buffer[i] == 10 && count == 4){
      break;
    }
    i++;
  }

  if(strength > 0){
    armys[numArmys].x = xPos;
    armys[numArmys].y = yPos;
    armys[numArmys].team = team;
    numArmys++;
    int radius = 2 + strength/10;
    if(radius > 4) radius = 4;
    tft.fillCircle(xPos, yPos, radius, teamColor); // draw unit
  }

  Serial.print("A\n");
  Serial.flush();
}


void updateGame(){
  tft.fillScreen(backgroundColor);

  shared.readUnits = true;
  while(shared.readUnits){
    read_line();
  }

  shared.readBuildings = true;
  while(shared.readBuildings){
    read_line();
  }


}

void winScreen(int winner){
  tft.fillScreen(customYell);
  tft.setCursor(100, 50); // reset other text options
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("Player ");
  tft.print(winner);
  tft.println(" WON!");
  tft.println("Please reset Arduino");
  tft.println("and Server");
  tft.println("to play again!");
  while(1){}
}

void playerTurn(int turn){
  int partOfTurn = 0;
  int selectBuild = 0;
  int startBuild;
  int prevBuild = 0;

  int winCheck = selectBuild;
  while(town[selectBuild].team != turn){ // make sure selected building is under player control
    selectBuild++;
    if(selectBuild >= numBuilds) selectBuild = 0;
    if(selectBuild == winCheck){ // if there are no more buildings other player wins
      if(turn == 1){
        winScreen(2);
      }
      else{
        winScreen(1);
      } 
    }
  }
  startBuild = selectBuild;

  tft.drawRect(town[selectBuild].x - 3, town[selectBuild].y - 3, 26, 12, customYell); // circle first building

  while(partOfTurn < 2){
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;
    process_input(); // read if buttons are pushed

    if(partOfTurn == 1){
      if(shared.move_pushed){
        prevBuild = selectBuild;
        selectBuild++;
        if(selectBuild >= numBuilds) selectBuild = 0; // if we went through list of all buildings restart

        tft.drawRect(town[prevBuild].x - 3, town[prevBuild].y - 3, 26, 12, backgroundColor); // decircle previous building
        tft.drawRect(town[selectBuild].x - 3, town[selectBuild].y - 3, 26, 12, customYell); // circle next building
      }
      if(shared.action_pushed){
        moveToBuild = selectBuild;
        partOfTurn = 2; // move onto next part
      }
    }


    if(partOfTurn == 0){
      if(shared.move_pushed){
        prevBuild = selectBuild;
        selectBuild++;
        while(town[selectBuild].team != turn){ // make sure selected building is under player control
          selectBuild++;
          if(selectBuild >= numBuilds) selectBuild = 0;
        }
        tft.drawRect(town[prevBuild].x - 3, town[prevBuild].y - 3, 26, 12, backgroundColor); // decircle previous building
        tft.drawRect(town[selectBuild].x - 3, town[selectBuild].y - 3, 26, 12, customYell); // circle next building 
      }
      if(shared.select_pushed){
        if(town[selectBuild].selected == true){
          town[selectBuild].selected = false;
          tft.drawRect(town[selectBuild].x - 5, town[selectBuild].y - 5, 30, 16, backgroundColor); // deselect building
        }
        else{
          town[selectBuild].selected = true;
          tft.drawRect(town[selectBuild].x - 5, town[selectBuild].y - 5, 30, 16, customWhite); // select building
        }
      }
      if(shared.action_pushed){
        Serial.println("N");
        Serial.flush();
        shared.readData = true; // make sure server got the data before sending more
        while(shared.readData) read_line();
        partOfTurn = 1;
      }
    }
  }
}

void resetTown(){
  for(int i = 0; i < numBuilds; i++){
    town[i].team = 0;
    town[i].x = 0;
    town[i].y = 0;
    town[i].selected = 0;
  }
}

void sendData(){
  for(int i = 0; i < numBuilds; i++){
    if(town[i].selected){
      Serial.print("S ");
      Serial.println(i);
      Serial.flush();
      shared.readData = true; // make sure server got the data before sending more
      while(shared.readData) read_line();
    }
  }
  resetTown();
  Serial.print("T ");
  Serial.println(moveToBuild);
  Serial.flush();
  shared.readData = true; // make sure server got the data before sending more
  while(shared.readData) read_line();

  Serial.print("E\n");
  Serial.flush();
  shared.readData = true; // make sure server got the data
  while(shared.readData) read_line();

  numBuilds = 0;
}