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
extern GUI_CONST_STORAGE GUI_BITMAP bmEasy;
extern GUI_CONST_STORAGE GUI_BITMAP bmHard;
extern GUI_CONST_STORAGE GUI_BITMAP bmAbsurd;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game Info~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

GameInfo gameInfo;
int turn = 0;

 void setupGameInfo(){
	  gameInfo.frameCount = 1;
		gameInfo.score = 1;
		gameInfo.alive = true; 
	 	if(gameInfo.difficulty == 0){
		  gameInfo.pipeGap = 140;
		  gameInfo.pipeDistance = 160;
		}else if(gameInfo.difficulty == 1){
			gameInfo.pipeGap = 100;
			gameInfo.pipeDistance = 80;
		}else if(gameInfo.difficulty == 2){
			gameInfo.pipeGap =80;
			gameInfo.pipeDistance = 70;
		}
}

void setDifficulty(int choice){
	 gameInfo.difficulty = choice;
}

void updateScores(){
		int arraySize;
	  int i;
	  gameInfo.alive = false;
		queueDestroy(gameInfo.que);
		free(gameInfo.birdy);
		if(gameInfo.score>gameInfo.highScore.score){
			gameInfo.highScore.score = gameInfo.score;
			gameInfo.highScore.difficulty = gameInfo.difficulty;
		}
		
		// Orders the scores so the most recent are displayed first
		
		arraySize = sizeof(gameInfo.scores)/sizeof(gameInfo.scores[0]);
		for(i=arraySize-1; i>=0; i--){
			gameInfo.scores[i+1] = gameInfo.scores[i];
		}
		gameInfo.scores[0].score = gameInfo.score;
		gameInfo.scores[0].difficulty = gameInfo.difficulty;
		
		gameInfo.score = 1;
		turn ++;
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
	pipe->up = false;
	enq(gameInfo.que, pipe);
}

void updateAllPipes(){
	int i=0;
	int arraySize;
	Pipe *currentPipe;
	if(gameInfo.que->head->x <-50){
		deq(gameInfo.que);
		gameInfo.score++;
	}
	for(currentPipe = gameInfo.que->head; currentPipe != 0; currentPipe = currentPipe->next){
		 currentPipe->x = currentPipe->x-2;
	}	
  if(gameInfo.frameCount %gameInfo.pipeDistance == 0){
			initPipes();
	}
	if(hits(gameInfo.que->head)){	
		updateScores();
	}
	
	// moving pipes for absurd difficulty
	if(gameInfo.difficulty==2){
	   for(currentPipe = gameInfo.que->head; currentPipe != 0; currentPipe = currentPipe->next){
				if(!currentPipe->up){
					currentPipe->bottomY=  currentPipe->bottomY+1;
					currentPipe->topY=  currentPipe->topY+1;
					if(currentPipe->bottomY > 250){
						currentPipe->up = true;
					}
			  }else if(currentPipe->up){
					currentPipe->bottomY=  currentPipe->bottomY-1;
					currentPipe->topY=  currentPipe->topY-1;
					if(currentPipe->topY < 20){
						currentPipe->up = false;
					}
				}
	   }	
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

	if(gameInfo.birdy->velocity > 3){
		gameInfo.birdy->velocity = 3;
	}
	if(gameInfo.birdy->velocity < -4){
		gameInfo.birdy->velocity = -4;
	}
	if(gameInfo.birdy->y > 272){
		updateScores();
	}	
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Coin~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void initCoin(){
	int randomY= rand() % 240;
	// ensures the coin is between the pipes
	gameInfo.coin->x = gameInfo.que->tail->x+gameInfo.pipeDistance;
	gameInfo.coin->y = randomY;
}


void updateCoin(){
	gameInfo.coin->x = gameInfo.coin->x -2;
	if(abs(gameInfo.birdy->x-gameInfo.coin->x)<4){
		gameInfo.score=gameInfo.score+3;
		gameInfo.coin->x=-20;
	}
	if(gameInfo.frameCount %1170 == 0){
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
	int arraySize;
	GUI_Clear();
	GUI_DrawBitmap(&bmBackground, 0,0);
	sprintf(str, "%d is the score to beat", gameInfo.highScore.score);
	GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_Font8x16x1x2);
	
	// print strings if there is a highscore
	if(gameInfo.highScore.score > 0){
	if(gameInfo.highScore.difficulty == 0)
			GUI_SetColor(GUI_GREEN);
	if(gameInfo.highScore.difficulty == 1)
			GUI_SetColor(GUI_ORANGE);
	if(gameInfo.highScore.difficulty == 2)
			GUI_SetColor(GUI_RED);		
		GUI_DispStringHCenterAt(str, LCD_GetXSize()/2 , 30);
		GUI_SetColor(GUI_BLACK);
		GUI_DispStringHCenterAt("Previous Scores:", LCD_GetXSize()/2 , 70);
		
		arraySize = sizeof(gameInfo.scores)/sizeof(gameInfo.scores[0]);
		for(i = 0; i<arraySize;i++){
			if(gameInfo.scores[i].score!= NULL){
				if(gameInfo.scores[i].difficulty == 0)
					GUI_SetColor(GUI_GREEN);
				if(gameInfo.scores[i].difficulty == 1)
					GUI_SetColor(GUI_ORANGE);
				if(gameInfo.scores[i].difficulty == 2)
					GUI_SetColor(GUI_RED);				
				sprintf(scores, "%d", gameInfo.scores[i].score);	
				GUI_DispStringHCenterAt(scores, LCD_GetXSize()/2 , listPos);
				listPos += 40;
			}
		}
	}
	// displays the difficulty buttons
	
	GUI_DrawBitmap(&bmEasy, 10,50);
	GUI_DrawBitmap(&bmHard, 10,110);
	GUI_DrawBitmap(&bmAbsurd, 10, 170);
	
}

void displayLeaderboard(TOUCH_STATE  tsc_state, GUI_RECT Rect){
	bool buttonPressed= false;	
	Touch_GetState (&tsc_state);
	while(!buttonPressed){
	    GUI_MEMDEV_Draw(&Rect, &drawHighscores, 0, 0, 0);
	    Touch_GetState (&tsc_state);
			if(tsc_state.pressed){
			if(tsc_state.x >= 10 && tsc_state.x <=90){
				if(tsc_state.y >=50 && tsc_state.y <=100){
						buttonPressed = true;
				    setDifficulty(0);
				}
				if(tsc_state.y >=110 && tsc_state.y <=160){
						buttonPressed = true;
					  setDifficulty(1);
				}
				if(tsc_state.y >=170 && tsc_state.y <= 230){
						buttonPressed = true;
					  setDifficulty(2);
				}
		}
	}
  }
	// finding which difficulty is chosen
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
	GUI_SelectLayer(3);
	
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringHCenterAt("GET READY!", 300, 74);
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

static void drawGame(void * pData){
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
	GUI_MEMDEV_Draw(&Rect, &drawGame, gameInfo.que, 0, 0);	
	gameInfo.frameCount ++;
}

