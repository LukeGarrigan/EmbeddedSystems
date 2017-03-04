/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.36 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : main.c
Purpose     : Main program Template
---------------------------END-OF-HEADER------------------------------
*/

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "RTE_Components.h"
#include "GUI.h"
#include <stdlib.h>
#include "main.h"
#include "Board_Touch.h" 
extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;
extern GUI_CONST_STORAGE GUI_BITMAP bmBackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmCoin;
extern GUI_CONST_STORAGE GUI_BITMAP bmPregame;
static int frameCount =1;
FILE *file;

//Bird bird;
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;
Coin coin;
GameInfo gameInfo;
Difficulty difficulty;
uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  *   System Clock source            = PLL (HSE)
  *   SYSCLK(Hz)                     = 200000000
  *   HCLK(Hz)                       = 200000000
  *   AHB Prescaler                  = 1
  *   APB1 Prescaler                 = 4
  *   APB2 Prescaler                 = 2
  *   HSE Frequency(Hz)              = 25000000
  *   PLL_M                          = 25
  *   PLL_N                          = 400
  *   PLL_P                          = 2
  *   PLL_Q                          = 8
  *   VDD(V)                         = 3.3
  *   Main regulator output voltage  = Scale1 mode
  *   Flash Latency(WS)              = 6
  */
static void SystemClock_Config (void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  HAL_PWREx_EnableOverDrive();
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
}

/**
  * CPU L1-Cache enable
  */
static void CPU_CACHE_Enable (void) {

  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
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
	//GUI_DrawBitmap(&bmCoin, coin.x, coin.y);
	// for printing the score
	GUI_SelectLayer(2);
	GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
	GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font8x16x1x2);
	GUI_DispStringHCenterAt(str, LCD_GetXSize()/2 , 70);
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


static void drawHighscores(void * pData){
	int c;
	GUI_Clear();
	GUI_DrawBitmap(&bmBackground, 0,0);
	//file = fopen("HighScores.txt", "r");
	
	//if (file) {
  //  while ((c = getc(file)) != EOF)
     
  //  fclose(file);
	//}
	
} 


void initPipes(){
	Pipe *pipe = malloc(sizeof(Pipe));
	int screenHeight = 272; // height of screen
	int gap = 120;
	int min = 20; 	// minimum height of either pipe
	int forRand = screenHeight - gap - min; 
	int randTop= rand() % forRand+min;
	pipe->topY = randTop;
	pipe->bottomY = randTop+gap;
	pipe->x = 600;
	pipe->speed = 5;
	enq(gameInfo.que, pipe);
}

bool hits(Pipe * p){
	if(gameInfo.birdy->y < p->topY || gameInfo.birdy->y > p->bottomY){
		if(gameInfo.birdy->x > p->x && gameInfo.birdy->x < p->x + 50){
			return true;
		}
	}
	return false;
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
	  gameInfo.score = 0;
		free(gameInfo.birdy);
		queueDestroy(gameInfo.que);

	}
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

void displayLeaderboard(TOUCH_STATE  tsc_state, GUI_RECT Rect){
	Touch_GetState (&tsc_state);
	while(!tsc_state.pressed){
	    GUI_MEMDEV_Draw(&Rect, &drawHighscores, 0, 0, 0);
	    Touch_GetState (&tsc_state);
  }
}

void initGame(){
		
		initBird();
	 // initCoin();
		gameInfo.score = 0;
		gameInfo.alive = true;
		initPipes();
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

void MainTask(void) {
	int i =0;
	GUI_RECT Rect = {0, 0, 602,272};
	TOUCH_STATE  tsc_state;
  


	while(1){
	gameInfo.que = queueCreate();	
	initGame();	
	displayLeaderboard(tsc_state,Rect);
	Touch_GetState (&tsc_state);
	GUI_Delay(500);
	tsc_state.pressed=false;
	initPregame(tsc_state,Rect);
	frameCount = 1;
	// loop while the bird is still alive
  while (gameInfo.alive) {
		
		updateCoin();
		updateAllPipes();
		updateBirdy();
		/*
	
		*/
		GUI_MEMDEV_Draw(&Rect, &drawRectangle, gameInfo.que, 0, 0);
		
		frameCount ++;
	}
}
} 

	
	
	
	
	
	
	

/*********************************************************************
*
*       Main
*/
int main (void) {
		CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */
  GUI_Init();

  Touch_Initialize();
  MainTask();
  for (;;);
}

/*************************** End of file ****************************/
