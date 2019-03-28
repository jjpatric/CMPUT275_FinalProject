
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
extern int customPink;

extern Adafruit_ILI9341 tft;

buildings town[30]; // max number of buildings is 30
int numBuilds = 0;

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
  town[numBuilds].exists = true;
  numBuilds++;
  drawBuilding(type, pop, teamColor, xPos, yPos);


  Serial.print("A\n");
  Serial.flush();
}


void updateGame(){

    shared.readBuildings = true;

  while(shared.readBuildings){
    read_line();
  }

    // TODO: read and draw new unit positions
}


void playerTurn(){
int partOfTurn = 0;
int selectBuild = 0;
int startBuild;
int prevBuild = 0;

while(town[selectBuild].team != 1){ // make sure selected building is under player control
    selectBuild++;
}
startBuild = selectBuild;

tft.drawRect(town[selectBuild].x - 3, town[selectBuild].y - 3, 26, 12, customYell); // select first building

while(partOfTurn < 2){
    shared.select_pushed = 0;
    shared.action_pushed = 0;
    shared.move_pushed = 0;
    process_input(); // read if buttons are pushed

    if(partOfTurn == 0){
        if(shared.move_pushed){
            prevBuild = selectBuild;
            selectBuild++;

            while(town[selectBuild].team != 1){ // make sure selected building is under player control
                selectBuild++;
                if(selectBuild >= numBuilds) selectBuild = startBuild;
            }

            tft.drawRect(town[selectBuild].x - 3, town[selectBuild].y - 3, 26, 12, customYell); // select next building
            tft.drawRect(town[prevBuild].x - 3, town[prevBuild].y - 3, 26, 12, backgroundColor); // deselect previous building
  
        }
        if(shared.select_pushed){
            if(town[selectBuild].selected == true){
                town[selectBuild].selected = false;
                tft.drawRect(town[selectBuild].x - 5, town[selectBuild].y - 5, 30, 16, backgroundColor); // deselect building
            }
            else{
                town[selectBuild].selected = true;
                tft.drawRect(town[selectBuild].x - 5, town[selectBuild].y - 5, 30, 16, customPink); // select building
            }

        }
        if(shared.action_pushed){
            partOfTurn++;
        }


    }
    if(partOfTurn == 1){


    }


}

numBuilds = 0;
}