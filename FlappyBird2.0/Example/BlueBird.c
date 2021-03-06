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
 * @file BlueBird.c
 * @author Luke Garrigan, Shane Sturgeon
 * @date 17 March 2017
 * @brief bitmap image of bird player
 */

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmBlueBird;

static GUI_CONST_STORAGE unsigned long _acBlueBird[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0x004C3C51, 0x004C3C51, 0x004C3C51, 0x004C3C51, 0x00D4A642, 0x00D4A642, 0x00FBD055, 0x00FBD055, 0x00FBD055, 0x00FBD055, 0x00463752, 0x00463752, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0x004C3C51, 0x004C3C51, 0x004C3C51, 0x004C3C51, 0x00D4A642, 0x00D4A642, 0x00FBD055, 0x00FBD055, 0x00FBD055, 0x00FBD055, 0x00463752, 0x00463752, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0xFFD1A341, 0xFFD1A341, 0x004C3C51, 0x004C3C51, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D4A542, 0x00D4A542, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0xFFD1A341, 0xFFD1A341, 0x004C3C51, 0x004C3C51, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D4A542, 0x00D4A542, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D4A542, 0x00D4A542, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 0x00CCE5D6, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFD1A341, 0xFFD1A341, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D4A542, 0x00D4A542, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 0x00CCE5D6, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 0x00CCE5D6, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 0x00CCE5D6, 0x00FAFAFA, 
        0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 
        0x00CCE5D6, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x00CCE5D6, 
        0x00CCE5D6, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FAFAFA, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x002E60E4, 
        0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x00463752, 0x00463752,
  0x00463752, 0x00463752, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00463752, 0x00463752, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00F8C14A, 0x00463752, 0x00463752, 0x002E60E4, 
        0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x00463752, 0x00463752,
  0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 0x00463752, 0x002E60E4, 0x002E60E4, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x0083D8FC, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 0x00463752, 0x002E60E4, 0x002E60E4, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 0x00463752, 0x002E60E4, 
        0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 0x00463752, 0x002E60E4, 
        0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x002E60E4, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00D1A341, 0x00463752, 
        0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0x00463752, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

GUI_CONST_STORAGE GUI_BITMAP bmBlueBird = {
  34, // xSize
  24, // ySize
  136, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_acBlueBird,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP8888
};

/*************************** End of file ****************************/
