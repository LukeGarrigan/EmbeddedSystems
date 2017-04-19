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
 * @file main.c
 * @author Luke Garrigan, Shane Sturgeon
 * @date 17 March 2017
 * @brief File containing all setup methods to get the game initialised
 */
 
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "RTE_Components.h"
#include "GUI.h"
#include <stdlib.h>
#include "main.h"
#include "Board_Touch.h" 

FILE *file;


#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

//Points Coin
Coin coin;

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


/**
 * @brief Initialise the game state.
 * @param  None
 * @retval None
 *
 * Calling @b initGame() creates the initial game state by
 * creating a bird, setting all game variables, creating the 
 * pipe queue and adding an extra points coin in.
 */
void initGame(){	
	  initBird();	
		setupGameInfo();
		initPipes();
	  initCoin();
}

/**
 * @brief Task container
 * @param  None
 * @retval None
 *
 * Holds all method calls to run the game, a while loop to setup 
 * the game and an inner loop to update all positions of elements 
 * on screen whilst the bird is alive
 */
void MainTask(void) {
	GUI_RECT Rect = {0, 0, 602,272};
	TOUCH_STATE  tsc_state;

	while(1){	
	createPipeQueue();	
	displayLeaderboard(tsc_state,Rect);
	Touch_GetState (&tsc_state);
	GUI_Delay(500);
	tsc_state.pressed=false;
  
	initGame();
	initPregame(tsc_state,Rect);	
	// loop while the bird is still alive
  while (isbirdAlive()) {	
		drawEverything(Rect);
		updateCoin();
		updateAllPipes();
		updateBirdy();
	}
}
} 

/**
 * @brief Main
 * @param  None
 * @retval int
 *
 * Configures all hardware used and calls @b MainTask
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
