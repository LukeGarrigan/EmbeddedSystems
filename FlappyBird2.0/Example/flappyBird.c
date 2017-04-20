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
#include <math.h>
extern GUI_CONST_STORAGE GUI_BITMAP bmCowboyBirdy;
extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;
extern GUI_CONST_STORAGE GUI_BITMAP bmKingBirdy;
extern GUI_CONST_STORAGE GUI_BITMAP bmBackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmCoin;
extern GUI_CONST_STORAGE GUI_BITMAP bmPregame;
extern GUI_CONST_STORAGE GUI_BITMAP bmEasy;
extern GUI_CONST_STORAGE GUI_BITMAP bmHard;
extern GUI_CONST_STORAGE GUI_BITMAP bmAbsurd;
extern GUI_CONST_STORAGE GUI_BITMAP bmArrow;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo2;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game Info~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

GameInfo gameInfo;
int turn = 0;
bool xpSet = false;



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
	  if(xpSet==false){
				gameInfo.playerLevel = malloc(sizeof(PlayerLevel));
			  gameInfo.playerLevel->playerLevel = 1;
				gameInfo.playerLevel->currentXp = 1;
				gameInfo.playerLevel->requiredXp = 20;
				xpSet=true;
		}
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
		//Increase score multiplier for different difficulties
		if(gameInfo.difficulty == 0){
			gameInfo.playerLevel->currentXp += gameInfo.score;
		}else if(gameInfo.difficulty == 1){
			gameInfo.playerLevel->currentXp += gameInfo.score * 2;
		}else if(gameInfo.difficulty == 2){
			gameInfo.playerLevel->currentXp += gameInfo.score * 3;
		}

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
		if(gameInfo.birdy->x > p->x && gameInfo.birdy->x < p->x + 70){
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
	if(hits(gameInfo.que->head)){	
		updateScores();
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


void drawProgress(){
	char lvl[30];
	char percentToLvl[30];
	float circleFill = (((float)gameInfo.playerLevel->currentXp/(float)gameInfo.playerLevel->requiredXp)*100)/5;
	int ceilCircle = ceil(circleFill);

	if(ceilCircle >= 20){
			int difference = ceilCircle -20;
			int base = gameInfo.playerLevel->requiredXp;
			gameInfo.playerLevel->playerLevel = gameInfo.playerLevel->playerLevel+1;
			gameInfo.playerLevel->currentXp =difference;
			gameInfo.playerLevel->requiredXp = base*2;
	}
	GUI_SetColor(GUI_WHITE);	
	GUI_FillCircle(455,25,20);
	GUI_SetColor(GUI_GREEN);
	GUI_FillCircle(455,25,ceilCircle);
	
	
	sprintf(percentToLvl, "%d%%", ceilCircle*5);
	GUI_SetColor(GUI_GREEN);
	GUI_SetFont(&GUI_Font8x16x1x2);
	GUI_DispStringHCenterAt(percentToLvl, 416 , 10);
	
	
	sprintf(lvl, "Level: %d", gameInfo.playerLevel->playerLevel);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font8x16x3x3);
	GUI_DispStringHCenterAt(lvl, 370 , 230);

}

void drawBirdChoice(){
	GUI_DrawBitmap(&bmBlueBird, 250, 185);
	GUI_SetColor(GUI_GRAY);
	if(gameInfo.birdType==0){
			GUI_FillRect(250,214, 285, 217);
	}else if(gameInfo.birdType==1){
			GUI_FillRect(320,214, 355, 217);
	}else if(gameInfo.birdType==2){
			GUI_FillRect(390,214, 425, 217);
	}
		
		
	GUI_SetColor(GUI_RED);
	GUI_SetFont(&GUI_Font8x16x1x2);


	
	if(gameInfo.playerLevel->playerLevel<5){
		GUI_DispStringHCenterAt("lvl 5:", 410 , 150);
		if(gameInfo.playerLevel->playerLevel<3){
			GUI_DispStringHCenterAt("lvl 3:", 340 , 150);
		}
	}
	
  GUI_DrawBitmap(&bmCowboyBirdy, 320, 181);

	GUI_DrawBitmap(&bmKingBirdy, 390, 181);
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
void drawHighscores(void * pData){
	

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
			drawProgress();
			drawBirdChoice();
		
		
		
		
		if(gameInfo.highScore.difficulty == 0){
				GUI_SetColor(GUI_GREEN);
		}
		if(gameInfo.highScore.difficulty == 1){
				GUI_SetColor(GUI_ORANGE);
		}
		if(gameInfo.highScore.difficulty == 2){
			GUI_SetColor(GUI_RED);	
		}

	
	  GUI_SetFont(&GUI_Font8x16x1x2);
		GUI_DispStringHCenterAt(str, 350 , 80);

		GUI_SetColor(GUI_BLACK);
		GUI_DispStringHCenterAt("Previous Scores:", 170 , 80);
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
				GUI_DispStringHCenterAt(scores, 150 , listPos);
				listPos += 40;
			}
		}
	}else{
			// there has been no scores yet
			GUI_DispStringHCenterAt("I'd probably do easy if I were you..", LCD_GetXSize()/2+30 , LCD_GetYSize()/2);
			GUI_DrawBitmap(&bmArrow, 110, 58);
			
		
	}
	// displays the difficulty buttons and the experience bar
	GUI_DrawBitmap(&bmEasy, 10,50);
	GUI_DrawBitmap(&bmHard, 10,110);
	GUI_DrawBitmap(&bmAbsurd, 10, 170);
	GUI_DrawBitmap(&bmlogo2,150,20);
	
	

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
				
					// GUI_DrawBitmap(&bmCowboyBirdy, 320, 181);
					//GUI_DrawBitmap(&bmKingBirdy, 390, 181);
					
					if(tsc_state.y >=181 && tsc_state.y <=250){
					// blue birdy
						if(tsc_state.x >=250 && tsc_state.x <=284){
							gameInfo.birdType = 0;
						}
						
						if(gameInfo.playerLevel->playerLevel >=3){
							if(tsc_state.x >=320 && tsc_state.x <=354){ //
								gameInfo.birdType = 1;
							}
							
							if(gameInfo.playerLevel->playerLevel >=5){
								if(tsc_state.x >=390 && tsc_state.x <=424){ //
									gameInfo.birdType = 2;
							}
								
						}
						
					}
				 
				
				
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
					
				
				if(gameInfo.birdType != 2 && gameInfo.birdType !=1){
					gameInfo.birdType = 0;
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
	
	
	if(gameInfo.birdType ==0){
		GUI_DrawBitmap(&bmBlueBird, gameInfo.birdy->x, gameInfo.birdy->y);
	}else if(gameInfo.birdType ==1){
		GUI_DrawBitmap(&bmCowboyBirdy, gameInfo.birdy->x, gameInfo.birdy->y);
	}else if(gameInfo.birdType ==2){
		GUI_DrawBitmap(&bmKingBirdy, gameInfo.birdy->x, gameInfo.birdy->y);
	}
	
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
	
	if(gameInfo.birdType ==0){
		GUI_DrawBitmap(&bmBlueBird, gameInfo.birdy->x, gameInfo.birdy->y);
	}else if(gameInfo.birdType ==1){
		GUI_DrawBitmap(&bmCowboyBirdy, gameInfo.birdy->x, gameInfo.birdy->y);
	}else if(gameInfo.birdType ==2){
		GUI_DrawBitmap(&bmKingBirdy, gameInfo.birdy->x, gameInfo.birdy->y);
	}
	
	
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

