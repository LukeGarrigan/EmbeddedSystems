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
void initBirds(void){
	int xSize = LCD_GetXSize();
  int ySize = LCD_GetYSize();
  int xPos = xSize / 15;
  int yPos = ySize / 5;

	bird.myBirdy = &bmBlueBird;
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
	pipe->x = 300;
	pipe->speed = 5;
	pipe->botSprite.bmBottomPipe= &bmBottomPipe; 
  pipe->botSprite.botSprite = GUI_SPRITE_Create(pipe->botSprite.bmBottomPipe, pipe->x ,170);

	pipe->topSprite.bmTopPipe= &bmTopPipe; 
  pipe->topSprite.topSprite = GUI_SPRITE_Create(pipe->topSprite.bmTopPipe, pipe->x,-70);
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
	
	if(pipeQueue->head->x <0){
		deq(pipeQueue);
	}
	//if(isOffScreen(pipeQueue)){
	//	deq(pipeQueue);
	//}
	for(currentPipe = thisGame.que.head; currentPipe != 0; currentPipe = currentPipe->next){
		currentPipe->x = currentPipe->x-1;
		GUI_SPRITE_SetPosition(currentPipe->botSprite.botSprite, currentPipe->x, 170);
	  GUI_SPRITE_SetPosition(currentPipe->topSprite.topSprite, currentPipe->x, -70);
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
	GUI_SPRITE_SetPosition(thisGame.bird.hsprite,thisGame.bird.xPos, thisGame.bird.yPos);
	//GUI_DrawBitmap(thisGame.bird.myBirdy,thisGame.bird.xPos, thisGame.bird.yPos);
	
}