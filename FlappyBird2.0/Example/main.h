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
 * @file main.h
 * @author Luke Garrigan, Shane Sturgeon
 * @date 17 March 2017
 * @brief header file containing all methods and structs
 */
#ifndef _MAIN_H
#define _MAIN_H
#include <stdbool.h>
#include "Board_Touch.h"




typedef struct {
  int Xpos;
  int Ypos;
	int Xpos2;
	int Ypos2;
	
} Rectangle;

/**
 * @brief Models a pipe
 *
 *  Structure to model all information needed to draw a pipe. 
 *  a pipe will be added to the pipeQueue
 */
typedef struct Pipe {
	 bool up;             /**< Weather the pipe is moving up or down (insane difficulty) */
   int x;   						/**< x co-ordinate*/
   int speed;				  	/**< Speed the pipe moves at*/
	 int topY;          	/**< Y position of the top half of the pipe */
	 int bottomY;      	  /**< Y position of the bottom half of the pipe */
	 struct Pipe *next;   /**< Pointer to the following piper */
}Pipe;

/**
 * @brief Queue of pipes
 *
 *  Creates a queue of pipes that is used by the game state
 */
typedef struct queue {
     Pipe *head;			/**< Points to the head pipe of the queue*/
     Pipe *tail;      /**< Points to the tail pipe of the queue*/
}queue ;

/**
 * @brief Models a bird
 *
 *  Structure to model all information needed to draw a bird.
 */
typedef struct Bird{
	int x;           	 /**< x co-ordinate of birds position*/
	int y;					 	 /**< y co-ordinate of birds position*/
  int gravity;       /**< Strength of the gravity */
	int velocity;      /**< Velocity speed */
	int lift;          /**< ? */
	bool up;					 /**< Weather the bird is moving up or down */
}Bird;

/**
 * @brief Models a coin
 *
 *  Structure to model all information needed to draw a coin.
 */
typedef struct Coin{
	int x; 						/**< x co-ordinate of coins position*/
	int y;						/**< y co-ordinate of coins position*/
}Coin;

/**
 * @brief All score information
 *
 *  Stores all information regarding score
 */
typedef struct Score{
	int score;       	/**< Score achieved*/
	int difficulty;		/**< Level of difficulty when score was achieved*/
}Score;	


/**
 * @brief Tracks the user's current level
 *
 *  Stores all game information regarding the current 
 *  level of the player.
 */
typedef struct PlayerLevel{
	int playerLevel;	/**< Stores level progress for user */
	int currentXp; 		/**< amount of xp on current level */  
	int requiredXp;   /**< amount of xp till next level */ 
}PlayerLevel;

/**
 * @brief Models Game State
 *
 *  Stores all game information regarding the current 
 *  state of the game.
 */
typedef struct GameInfo{
	Bird *birdy;      /**< Pointer to bird in play */
	queue * que;      /**< Pointer to queue containing all pipes */
	Coin * coin;      /**< Pointer to coin in play*/
	PlayerLevel * playerLevel; /**< Pointer to player level in play*/
	bool alive;       /**< Boolean check to see if the game is still playing*/
	int difficulty;		/**< Difficulty rating (0-2)*/
	int frameCount;   /**< Counter for frames*/
	int score;				/**< Current score of the game*/
	int pipeGap;			/**< Distance between top and bottom pipe */
	Score highScore;  /**< Highest score achieved in game state */
	int pipeDistance; /**< Distance between concurrent pipes */
	Score scores[4];  /**< Saves n most current scores */
	int birdType;
}GameInfo;


typedef struct Difficulty {
	GameInfo *gameinfo;
}Difficulty;

void queueDestroy(struct queue *q);
int queueEmpty(const struct queue *q);
void updateAllPipes(void);
void deq(queue *q);
void updatePipes(queue *q);
void erasePipes(queue *q); 
void enq(queue *q, Pipe *p);
queue * queueCreate(void);
Pipe* getList(int position);
bool isOffScreen(queue *q);
void initGame(void);
void initBird(void);
void initCoin(void);
void setupGameInfo(void);
void initPipes(void);
void displayLeaderboard(TOUCH_STATE  tsc_state, GUI_RECT Rect);
void initPregame(TOUCH_STATE  tsc_state, GUI_RECT Rect);
bool isbirdAlive(void);
void updateCoin(void);
void updateBirdy(void);
void drawEverything(GUI_RECT Rect);
void createPipeQueue(void);
void setDifficulty(int choice);
#endif /* _MAIN_H */
