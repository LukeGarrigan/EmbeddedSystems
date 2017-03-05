#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "RTE_Components.h"
#include "GUI.h"
#include <stdlib.h>
#include "main.h"


extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;
extern GUI_CONST_STORAGE GUI_BITMAP bmBackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmCoin;
extern GUI_CONST_STORAGE GUI_BITMAP bmPregame;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game Info~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

GameInfo gameInfo;
int frameCount;
int turn = 0;

 void setupGameInfo(){
	  frameCount = 1;
		gameInfo.score = 0;
		gameInfo.alive = true;
		gameInfo.pipeGap = 120;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pipes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void createPipeQueue(){
	gameInfo.que = queueCreate();
}

bool hits(Pipe * p){
	if(gameInfo.birdy->y < p->topY || gameInfo.birdy->y > p->bottomY){
		if(gameInfo.birdy->x > p->x && gameInfo.birdy->x < p->x + 50){
			return true;
		}
	}
	return false;
}

void initPipes(){
	Pipe *pipe = malloc(sizeof(Pipe));
	int screenHeight = 272; // height of screen
	int gap = gameInfo.pipeGap;
	int min = 20; 	// minimum height of either pipe
	int forRand = screenHeight - gap - min; 
	int randTop= rand() % forRand+min;
	pipe->topY = randTop;
	pipe->bottomY = randTop+gap;
	pipe->x = 600;
	pipe->speed = 5;
	enq(gameInfo.que, pipe);
}

void updateAllPipes(){
	Pipe *currentPipe;

	if(gameInfo.que->head->x <-50){
		deq(gameInfo.que);
		gameInfo.score++;
	}
	for(currentPipe = gameInfo.que->head; currentPipe != 0; currentPipe = currentPipe->next){
		 currentPipe->x = currentPipe->x-2;
	}	
  if(frameCount %150 == 0){
			initPipes();
	}
	if(hits(gameInfo.que->head)){	
	  gameInfo.alive = false;
		free(gameInfo.birdy);
		queueDestroy(gameInfo.que);
		if(gameInfo.score>gameInfo.highScore){
			gameInfo.highScore = gameInfo.score;
		}
		gameInfo.scores[turn%4] = gameInfo.score;
		gameInfo.score = 0;
		turn ++;
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bird~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool isbirdAlive(){
	return gameInfo.alive;
}

void initBird(){
	gameInfo.birdy = malloc(sizeof(Bird));
	gameInfo.birdy->x = 10;
	gameInfo.birdy->y = 100;
	gameInfo.birdy->gravity = 1;
	gameInfo.birdy->velocity = 1;
	gameInfo.birdy->up = false;

}
void upBirdy(void){
	gameInfo.birdy->velocity += - gameInfo.birdy->gravity*2;
}

void updateBirdy(void){
	TOUCH_STATE  tsc_state;
  Touch_GetState (&tsc_state);
	if(tsc_state.pressed){
		upBirdy();	
	}
	//bird.velocity *=0.7;
	gameInfo.birdy->velocity += gameInfo.birdy->gravity;
	gameInfo.birdy->y += gameInfo.birdy->velocity;
	if(gameInfo.birdy->y > 272){
		gameInfo.birdy->y = 272/2;
		gameInfo.score = 0;
	}	
	
	if(gameInfo.birdy->velocity > 3){
		gameInfo.birdy->velocity = 3;
	}
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Coin~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void initCoin(){
	gameInfo.coin->x = 600;
	gameInfo.coin->y = 186;
}


void updateCoin(){
	gameInfo.coin->x = gameInfo.coin->x -2;
	if(abs(gameInfo.birdy->x-gameInfo.coin->x)<4){
		gameInfo.score=gameInfo.score+3;
		gameInfo.coin->x=-20;
	}
	if(frameCount %1170 == 0){
			initCoin();
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~Displaying to Screen~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void drawHighscores(void * pData){
	char str[30];
	char scores[40];
	int i;
	int listPos = 110;
	GUI_Clear();
	GUI_DrawBitmap(&bmBackground, 0,0);
	sprintf(str, "%d is the score to beat", gameInfo.highScore);
	GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
  GUI_SetFont(&GUI_Font8x16x1x2);
	if(gameInfo.highScore > 0){
		GUI_DispStringHCenterAt(str, LCD_GetXSize()/2 , 30);
		GUI_DispStringHCenterAt("Previous Scores:", LCD_GetXSize()/2 , 70);
		for(i = 0; i<4;i++){
			if(gameInfo.scores[i]!= NULL){
				sprintf(scores, "%d", gameInfo.scores[i]);	
				GUI_DispStringHCenterAt(scores, LCD_GetXSize()/2 , listPos);
				listPos += 40;
			}
		}
	}
	//file = fopen("HighScores.txt", "r");
	
	//if (file) {
  //  while ((c = getc(file)) != EOF)
     
  //  fclose(file);
	//}
	
}

void displayLeaderboard(TOUCH_STATE  tsc_state, GUI_RECT Rect){
	Touch_GetState (&tsc_state);
	while(!tsc_state.pressed){
	    GUI_MEMDEV_Draw(&Rect, &drawHighscores, 0, 0, 0);
	    Touch_GetState (&tsc_state);
  }
}


// Draws the pregame, so prompts the user that they 
// have to tap/hold down to make the bird fly!
static void drawPregrame(void * pData){
	GUI_Clear();
	GUI_DrawBitmap(&bmBackground, 0,0);
	GUI_SelectLayer(1);
	GUI_DrawBitmap(&bmPregame, 80, 95);
	GUI_SelectLayer(2);
	GUI_DrawBitmap(&bmBlueBird, gameInfo.birdy->x, gameInfo.birdy->y);
} 

void initPregame(TOUCH_STATE  tsc_state, GUI_RECT Rect){
	while(!tsc_state.pressed){
		if(gameInfo.birdy->up == false){
			gameInfo.birdy->y--;
			if(gameInfo.birdy->y < 100){
			    gameInfo.birdy->up = true;
			}
		}else if(gameInfo.birdy->up == true){
			gameInfo.birdy->y++;
			if(gameInfo.birdy->y > 120){
				gameInfo.birdy->up = false;
			}
		}
		GUI_MEMDEV_Draw(&Rect, &drawPregrame, 0, 0, 0);
		Touch_GetState (&tsc_state);
	}	
}

static void drawRectangle(void * pData){
	//static int i =0;
	char str[15];
	Pipe *currentPipe;
	sprintf(str, "%d", gameInfo.score);
	GUI_Clear();
	GUI_DrawBitmap(&bmBackground, 0,0);
	
	GUI_SelectLayer(1);
	GUI_SetColor(GUI_BROWN);
	for(currentPipe = gameInfo.que->head; currentPipe != 0; currentPipe = currentPipe->next){
		GUI_FillRect(currentPipe->x, 0, currentPipe->x+50, currentPipe->topY);
		GUI_FillRect(currentPipe->x, currentPipe->bottomY, currentPipe->x+50, 272);
	}
	GUI_DrawBitmap(&bmBlueBird, gameInfo.birdy->x, gameInfo.birdy->y);
	GUI_DrawBitmap(&bmCoin, gameInfo.coin->x, gameInfo.coin->y);
	// for printing the score
	GUI_SelectLayer(2);
	GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
	GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font8x16x1x2);
	GUI_DispStringHCenterAt(str, LCD_GetXSize()/2 , 70);
}

void drawEverything(GUI_RECT Rect){
	GUI_MEMDEV_Draw(&Rect, &drawRectangle, gameInfo.que, 0, 0);	
	frameCount ++;
}

