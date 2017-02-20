
#ifndef _MAIN_H
#define _MAIN_H
#include <stdbool.h>

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
}Bird;


typedef struct Coin{
	int x;
	int y;
}Coin;

typedef struct GameInfo{
	Bird * birdy;
	queue * que;
	int score;
}GameInfo;



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