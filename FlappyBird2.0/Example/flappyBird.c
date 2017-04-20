/****************************************************************************
 * Copyright (C) 2017 by Shane Sturgeon, Luke Garrigan                      *
 *                                                                          *
 * This file is part of Flappy Birds.                                       *
 *                                                                          *
 *  Flappy Birds is a tap based game created on the STM32F746G discovery    *
 *  board as part of Embedded Systems Coursework                            *
 *                                                                          *
 ****************************************************************************/
/**
 * @file flappyBird.c
 * @author Luke Garrigan, Shane Sturgeon
 * @date 17 March 2017
 * @brief File containing all methods to update the gamestate 
 */
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "RTE_Components.h"
#include "GUI.h"
#include <stdlib.h>
#include "main.h"
#include "Progbar.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;
extern GUI_CONST_STORAGE GUI_BITMAP bmBackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmCoin;
extern GUI_CONST_STORAGE GUI_BITMAP bmPregame;
extern GUI_CONST_STORAGE GUI_BITMAP bmEasy;
extern GUI_CONST_STORAGE GUI_BITMAP bmHard;
extern GUI_CONST_STORAGE GUI_BITMAP bmAbsurd;
extern GUI_CONST_STORAGE GUI_BITMAP bmArrow;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game Info~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

GameInfo gameInfo;
int turn = 0;




/**
 * @brief SetupGameInfo, sets up game based on difficulty.
 * @param  None
 * @retval None
 *
 * Based on gameinfo struct difficulty variable this method
 * sets up the speed of the game, distance between concurrent
 * pipes and the gap between the middle of a pipe.
 */
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

/**
 * @brief Sets game difficulty
 * @param  int difficulty choice, num between 0-2
 * @retval None
 * @note 0 = Normal, 1 = Hard, 2 = Insane
 *
 * sets gameInfo struct game difficulty to later be used
 * by @b setupGameInfo 
 */
void setDifficulty(int choice){
	 gameInfo.difficulty = choice;
}

/**
 * @brief Updates the scoreboard
 * @param  None
 * @retval None
 *
 * Once a bird dies, this method updates the scoreboard in 
 * order of most recent 1st and what difficulty the score was 
 * achieved on 
 */
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

/**
 * @brief Assigns a pipe queue to the game state
 * @param  None
 * @retval None
 *
 * @b queueCreate methods is called and returned value
 * is saved within gameinfo structs queue
 */
void createPipeQueue(){
	gameInfo.que = queueCreate();
}

/**
 * @brief Detects collisions
 * @param  pointer to the head pipe
 * @retval bool True of False
 *
 * Takes in the head of the pipe queue to get the position on screen,
 * if the birds position is in the same co-ordinates true is returned
 * otherwise false.
 */
bool hits(Pipe * p){
	if(gameInfo.birdy->y < p->topY || gameInfo.birdy->y > p->bottomY){
		if(gameInfo.birdy->x > p->x && gameInfo.birdy->x < p->x + 50){
			return true;
		}
	}
	return false;
}

/**
 * @brief Initialises a pipe
 * @param  None
 * @retval None
 *
 * Creates a pipe and enques pipe to the game info
 * pipe queue
 */
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

/**
 * @brief Updates all pipes in the queue
 * @param  None
 * @retval None
 * @note also changes pipe gap size and position for insane dif
 *
 * udpates co-ordinates for all pipes currently in the pipequeue, 
 * as well as calling @b hits to see if bird is still alive
 */
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

/**
 * @brief If the bird is alive or not
 * @param  None
 * @retval bool true or false
 *
 * Returns the gameinfo struct alive boolean variable that is altered 
 * by @b updateScores 
 */
bool isbirdAlive(){
	return gameInfo.alive;
}

/**
 * @brief Initialises a bird
 * @param  None
 * @retval None
 *
 *  Initialises a bird and adds it to the game states
 */
void initBird(){
	gameInfo.birdy = malloc(sizeof(Bird));
	gameInfo.birdy->x = 10;
	gameInfo.birdy->y = 100;
	gameInfo.birdy->gravity = 1;
	gameInfo.birdy->velocity = 1;
	gameInfo.birdy->up = false;

}

/**
 * @brief Moves the bird up
 * @param  None
 * @retval None
 *
 *  Alters the bird position based on gravity, moving it up 
 */
void upBirdy(void){
	gameInfo.birdy->velocity += - gameInfo.birdy->gravity*4;
}

/**
 * @brief Updates the birds state 
 * @param  None
 * @retval None
 *
 * Updates the birds y positioning (up and down movements),
 * as well as checking the bird hasn't fell below the screen line
 * calling @b updatesScores and ending a game loop
 */
void updateBirdy(void){
	TOUCH_STATE  tsc_state;
  Touch_GetState (&tsc_state);
	if(tsc_state.pressed){
		upBirdy();	
	}
	//bird.velocity *=0.7;
	gameInfo.birdy->velocity += gameInfo.birdy->gravity;
	gameInfo.birdy->y += gameInfo.birdy->velocity;

	
	// down velocity
	if(gameInfo.birdy->velocity > 4){
		gameInfo.birdy->velocity = 4;
	}
	
	// up velocity
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

/**
 * @brief Initialises a coin
 * @param  None
 * @retval None
 *
 * Initialises a coin to a random position in the game state
 */
void initCoin(){
	int randomY= rand() % 240;
	// ensures the coin is between the pipes
	gameInfo.coin->x = gameInfo.que->tail->x+gameInfo.pipeDistance;
	gameInfo.coin->y = randomY;
}

/**
 * @brief Updates coin positioning
 * @param  None
 * @retval None
 *
 * Update coins x position to move it across the screen, calling 
 * @b initCoin when a new coin needs initalising (every 1170 frames)
 */
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

static void demoProgBar(void) {
	PROGBAR_Handle hProgBar;
	GUI_DispStringAt("Progress bar", 100, 20);
	hProgBar = PROGBAR_Create(100, 40, 100, 20, WM_CF_SHOW);
}



/**
 * @brief Displays scores and difficulty associated with that score
 * @param  None
 * @retval None
 * @note Normal = Green, Hard = Orange, Insane = Red
 *
 * Method that displays all scores saved within the game state, displaying 
 * newest score first in the colour of difficulty played. Also displays the 
 * game states highest score achieved.
 */
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
	}else{
			// there has been no scores yet
			GUI_DispStringHCenterAt("I'd probably do easy if I were you..", LCD_GetXSize()/2+30 , LCD_GetYSize()/2);
			GUI_DrawBitmap(&bmArrow, 110, 58);
			
		
	}
	// displays the difficulty buttons and the experience bar
	demoProgBar();
	GUI_DrawBitmap(&bmEasy, 10,50);
	GUI_DrawBitmap(&bmHard, 10,110);
	GUI_DrawBitmap(&bmAbsurd, 10, 170);
	
}




/**
 * @brief Creates a window to display scores and select difficulty
 * @param  TOUCH_STATE where the screen has been pressed, GUI_RECT where to display
 * @retval None
 *
 * Sets the game mode based on what button is pressed and 
 * calls @b drawHighScores to display scores and buttons. 
 */
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


/**
 * @brief Displays the how to play window
 * @param  None
 * @retval None
 *
 * Displays  an information screen informing the user how to 
 * play
 */
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

/**
 * @brief Initilise pregame
 * @param  TOUCH_STATE where the screen has been pressed, GUI_RECT where to display
 * @retval None
 *
 * Creates the window for the pregame to be drawn and waits for input
 */
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

/**
 * @brief Creates all images to be drawn
 * @param  None
 * @retval None
 *
 * Draws all elements of the game (coin,bird,pipeQueue,background)
 */
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

/**
 * @brief Allocates memory for drawing elements
 * @param  None
 * @retval None
 *
 * Allocates the memory for @b drawGame as well as incrementing
 * games frame-counter
 */
void drawEverything(GUI_RECT Rect){
	GUI_MEMDEV_Draw(&Rect, &drawGame, gameInfo.que, 0, 0);	
	gameInfo.frameCount ++;
}

