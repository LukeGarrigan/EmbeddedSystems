#include "main.h"
#include "GUI.h"
#include "Board_Touch.h" 
#include <stdio.h>
#include <stdlib.h>



extern GameInfo thisGame;
Birdy bird;
Pipe *pipe;
queue * pipeQueue;
extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;


void initBirds(void){
	int xSize = LCD_GetXSize();
  int ySize = LCD_GetYSize();
  int xPos = xSize / 15;
  int yPos = ySize / 5;

	bird.myBirdy = bmBlueBird;
	bird.xPos = xPos;
	bird.yPos = yPos;
	bird.gravity = 1;
	bird.velocity = 1;
	bird.lift = -10;
	thisGame.bird = bird;
}

void initPipes(void){
int ySize = 100;
	int randTop= rand() % ySize;
	int randBot = rand() % ySize+100;
	pipe = malloc(sizeof(Pipe));
	pipe->top =randTop;
	pipe->bottom = randBot;
	pipe->x = 600;
	pipe->speed = 5;
	enq(pipeQueue, pipe);
}


void initGame(void){
	initBirds();
	pipeQueue = queueCreate();
	initPipes();
	thisGame.num_ticks = T_SHORT;
	//initBirdy();
	
}
void updateAllPipes(){
	updatePipes(pipeQueue);
	if(isOffScreen(pipeQueue)){
		deq(pipeQueue);
	}
}
void drawAllPipes(){
	drawPipes(pipeQueue);
}

void upBirdy(void){
	thisGame.bird.velocity += - thisGame.bird.gravity*10;
}

void updateBirdy(void){
	TOUCH_STATE  tsc_state;
  Touch_GetState (&tsc_state);
	if(tsc_state.pressed){
		upBirdy();
	}else{
		thisGame.bird.velocity *=0.7;
		thisGame.bird.velocity += thisGame.bird.gravity*2;
		thisGame.bird.yPos += thisGame.bird.velocity;
	}

	GUI_DrawBitmap(&thisGame.bird.myBirdy,thisGame.bird.xPos, thisGame.bird.yPos);
	
}