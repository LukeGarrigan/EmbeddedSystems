#include "main.h"
#include "GUI.h"
#include "Board_Touch.h" 
#include <stdio.h>
#include <stdlib.h>



extern GameInfo thisGame;
Birdy bird;

queue * pipeQueue;
extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;
extern GUI_CONST_STORAGE GUI_BITMAP bmTopPipe;
extern GUI_CONST_STORAGE GUI_BITMAP bmBottomPipe;
extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird2;

void initBirds(void){
	int xSize = LCD_GetXSize();
  int ySize = LCD_GetYSize();
  int xPos = xSize / 15;
  int yPos = ySize / 5;
  bird.whichBird = 0;
	bird.myBirdy = &bmBlueBird;
	bird.myBirdy2 = &bmBlueBird2;
	bird.xPos = xPos;
	bird.yPos = yPos;
	bird.gravity = 1;
	bird.velocity = 1;
	bird.lift = -10;
	bird.hsprite = GUI_SPRITE_Create(bird.myBirdy,bird.xPos,bird.yPos);
	thisGame.bird = bird;
}

void initPipes(void){
	Pipe *pipe;
	int ySize = 100;
	int randTop= rand() % ySize;
	int randBot = rand() % ySize+100;

	
	pipe = malloc(sizeof(Pipe));
	pipe->topSprite.top =randTop;
	pipe->botSprite.bottom = randBot;
	pipe->x = 400;
	pipe->speed = 5;
	GUI_DrawBitmap(&bmTopPipe, pipe->x, 170);
	GUI_DrawBitmap(&bmBottomPipe, pipe->x,-70);
	enq(pipeQueue, pipe);
}


/*
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
*/

void initGame(void){
	initBirds();
	pipeQueue = queueCreate();
	initPipes();
	thisGame.num_ticks = T_SHORT;
	thisGame.que = *pipeQueue;
	
}
void updateAllPipes(){
	//updatePipes(pipeQueue);

	 Pipe *currentPipe;
	
	if(pipeQueue->head->x <50){
		deq(pipeQueue);
	}
	
	for(currentPipe = thisGame.que.head; currentPipe != 0; currentPipe = currentPipe->next){
		currentPipe->x = currentPipe->x-1;
		
		
		GUI_DrawBitmap(&bmTopPipe, currentPipe->x, 170);
		GUI_DrawBitmap(&bmBottomPipe, currentPipe->x,-70);
		}
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
	if(thisGame.bird.yPos > LCD_GetYSize()-30){
		thisGame.bird.yPos = LCD_GetYSize()-30;
	}
	GUI_SPRITE_SetPosition(thisGame.bird.hsprite,thisGame.bird.xPos, thisGame.bird.yPos);
	//GUI_DrawBitmap(thisGame.bird.myBirdy,thisGame.bird.xPos, thisGame.bird.yPos);
	
	
	
}












/*

	if(frameCount % 10==0){
	if(thisGame.bird.whichBird == 0){
		thisGame.bird.whichBird =1;
		GUI_SPRITE_Delete(thisGame.bird.hsprite);
		thisGame.bird.hsprite = GUI_SPRITE_Create(bird.myBirdy2 ,bird.xPos,bird.yPos);
	}else{
		thisGame.bird.whichBird =0;
		GUI_SPRITE_Delete(thisGame.bird.hsprite);
		thisGame.bird.hsprite = GUI_SPRITE_Create(bird.myBirdy ,bird.xPos,bird.yPos);
	}


*/
