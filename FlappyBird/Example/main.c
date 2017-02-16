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
#include "PipeLine.h"
#include "Birdy.h"
#include "main.h"
//#include "InitGame.c"
GameInfo thisGame;

static int count = 0;
//extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;



uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif




void drawPipes();
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
	int frameCount=0;
	//Pipe *p;
	Pipe *pipe;	
	//Pipe *secondPipe;
	//Pipe *thirdPipe;
	//queue * pipeQueue;
 // GUI_BITMAP bmbirdy;
	TOUCH_STATE tsc_state;
  int i = 0;
	int numPipes;
	GUI_RECT rect;
  CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */
	
  GUI_Init();
	initGame();
  Touch_Initialize();
	//GUI_DrawGradientV(0, 0, 480, 272, 0xffffff, 0xff0000);
  //GUI_SetBkColor(GUI_BLACK);
	//initBird(&bird);
	initBirdy();
	
	 // Initialising the pipe and the pipe queue
		//GUI_DrawBitmap(&thisGame.bird.myBirdy,thisGame.bird.xPos, thisGame.bird.yPos);
	//	GUI_DrawBitmap(&bmbackground,10, 10);
  while (1) {
		//GUI_DrawGradientRoundedV(10,10,70, 70,2, 0xffffff, 0xff0000);
		GUI_Delay(20);
		GUI_Clear();
		frameCount++;
		
		updateAllPipes();
		updateBirdy();
			
		if(frameCount % 50 == 0){
				initPipes();
		}
		//GUI_BITMAP 
		drawAllPipes();
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











/*************************** End of file ****************************/
