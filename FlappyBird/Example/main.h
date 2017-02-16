#ifndef _MAIN_H
#define _MAIN_H

#define wait_delay HAL_Delay
#include "GUI.h"
#include <stdbool.h>
#define T_SHORT 60

typedef struct Pipe {
	 int top;
   int bottom;
   int x;
   int speed;
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
	Pipe pipe;
} GameInfo;



void upBirdy(void);
void initBirdy(void);
void updateBirdy(void);

void 	drawAllPipes();
void updateAllPipes();
void initPipes();
void deq(queue *q);
void updatePipes(queue *q);
void erasePipes(queue *q); 
void drawPipes(queue *q);
void enq(queue *q, Pipe *p);
queue * queueCreate(void);
void drawPipes();
Pipe* getList(int position);
bool isOffScreen(queue *q);
void initGame(void);

#endif /* _MAIN_H */