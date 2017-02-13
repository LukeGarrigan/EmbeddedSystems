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
#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include <stdlib.h>
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;



uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif
typedef struct Pipe{
	int top;
	int bottom;
	int width;
	int x;
	int speed;
}Pipe;


typedef struct Birdy{
	int xPos;
  int yPos;	
	int gravity; 
	int velocity;
	int lift;
	
}Birdy;
void up(Birdy *bird);
void updateBird(Birdy *bird, TOUCH_STATE *tsc_state);
void initPipes(Pipe *pipe);
void initBird(Birdy *bird);
void updatePipes(Pipe *pipe);
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

/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
	Pipe pipe;
	TOUCH_STATE tsc_state;
  int xPos, yPos, xSize, ySize;
	int randTop, randBot;
  int i = 0;
	GUI_RECT rect;
	GUI_RECT pipeRect;
	Birdy bird;
  CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */

  GUI_Init();
  Touch_Initialize();
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
	
	initBird(&bird);
	
	// Initialising the pipe
  initPipes(&pipe);
	


  while (1) {
		Touch_GetState(&tsc_state);
		updatePipes(&pipe);
		updateBird(&bird, &tsc_state);
		GUI_DrawRect(bird.xPos, bird.yPos, bird.xPos+20, bird.yPos+20);
		GUI_DrawRect(pipe.x, 0, pipe.x+50, pipe.top);
	  GUI_DrawRect(pipe.x, pipe.bottom, pipe.x+50, LCD_GetYSize());
		for(i = 0; i< 3000000; i++){};
		GUI_ClearRect(pipe.x, pipe.bottom, pipe.x+50, LCD_GetYSize());
		GUI_ClearRect(pipe.x, 0, pipe.x+50, pipe.top);	
	  GUI_ClearRect(bird.xPos, bird.yPos, bird.xPos+20, bird.yPos+20);
  } 
}

/*********************************************************************
*
*       Main
*/
int main (void) {
  MainTask();
  for (;;);
}
void up(Birdy *bird){
	bird->velocity += - bird->gravity*10;
}

void updatePipes(Pipe *pipe){
	pipe->x -= pipe->speed;
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
	if(bird->yPos < 21){
		bird->yPos = 21;
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



void initPipes(Pipe *pipe){
	int ySize = 100;
	int randTop= rand() % ySize;
	int randBot = rand() % ySize+100;
	pipe->top =randTop;
	pipe->bottom = randBot;
	pipe->x = 100;
	pipe->speed = 5;
}



/*************************** End of file ****************************/
