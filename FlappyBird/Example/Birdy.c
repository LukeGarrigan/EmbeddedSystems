#include "Birdy.h"
#include "GUI.h"
#include "Board_Touch.h" 

void up(Birdy *bird){
	bird->velocity += - bird->gravity*3;
}
void updateBird(Birdy *bird, TOUCH_STATE *tsc_state){
	int screenHeight = LCD_GetYSize();
	if(tsc_state->pressed){
		up(bird);
	}else{
		bird->velocity += bird->gravity;
		bird->yPos += bird->velocity;
	}

	// ensures the bird doesn't go off the screen
	// also it's 21 as we want to see the bottom of the bird
	if(bird->yPos > screenHeight-21){
		bird->yPos = screenHeight-21;
		bird->velocity = 0;
	}
	if(bird->yPos < 0){
		bird->yPos = 0;
		bird->velocity = 0;
	}
}

void initBird(Birdy *bird){
	// Initialising the bird
	
  int xSize = LCD_GetXSize();
  int ySize = LCD_GetYSize();

  int xPos = xSize / 15;
  int yPos = ySize / 5;
	
	bird->xPos = xPos;
	bird->yPos = yPos;
	bird->gravity = 1;
	bird->velocity = 0;
	bird->lift = -10;

}
