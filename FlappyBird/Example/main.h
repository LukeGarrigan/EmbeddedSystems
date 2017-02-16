#ifndef _MAIN_H
#define _MAIN_H

#define wait_delay HAL_Delay
#include "GUI.h"
#include <stdbool.h>
#define T_SHORT 60




typedef struct TopSprite{
	 GUI_CONST_STORAGE GUI_BITMAP *bmTopPipe;
	 GUI_HSPRITE topSprite;
   int top;
}TopSprite;

typedef struct BotSprite{
	 GUI_CONST_STORAGE GUI_BITMAP *bmBottomPipe;
	 GUI_HSPRITE botSprite;	
   int bottom;
}BotSprite;

typedef struct Pipe {
   int x;
   int speed;
	 TopSprite topSprite;
	 BotSprite botSprite;
	 struct Pipe *next;
}Pipe ;


typedef struct queue {
     Pipe *head;
     Pipe *tail;
}queue ;


typedef struct{
	int xPos;
  int yPos;	
	int gravity; 
	int velocity;
	int lift;
	GUI_CONST_STORAGE GUI_BITMAP *myBirdy;
	GUI_HSPRITE hsprite;
}Birdy;


typedef struct{
	unsigned int num_ticks;
	Birdy bird;
	queue que;
} GameInfo;



void upBirdy(void);
void initBirdy(void);
void updateBirdy(void);
void initPipes(void);


void updateAllPipes(void);
void deq(queue *q);
void updatePipes(queue *q);
void erasePipes(queue *q); 
void enq(queue *q, Pipe *p);
queue * queueCreate(void);
Pipe* getList(int position);
bool isOffScreen(queue *q);
void initGame(void);

#endif /* _MAIN_H */