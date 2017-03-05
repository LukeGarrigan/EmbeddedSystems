
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

typedef struct Pipe {
   int x;
   int speed;
	 int topY;
	 int bottomY;
	 struct Pipe *next;
}Pipe;


typedef struct queue {
     Pipe *head;
     Pipe *tail;
}queue ;


typedef struct Bird{
	int x;
	int y;
  int gravity;
	int velocity;
	int lift;
	bool up;
}Bird;


typedef struct Coin{
	int x;
	int y;
}Coin;

typedef struct GameInfo{
	Bird *birdy;
	queue * que;
	Coin * coin;
	bool alive;
	int score;
	int pipeGap;
	int highScore;
	int scores[4];
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
#endif /* _MAIN_H */
