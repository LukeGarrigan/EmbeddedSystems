#ifndef PipeLine_H_INCLUDED
#define PipeLine_H_INCLUDED
#include <stdbool.h>
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

void deq(queue *q);
void updatePipes(queue *q);
void erasePipes(queue *q); 
void drawPipes(queue *q);
void enq(queue *q, Pipe *p);
queue * queueCreate(void);
void drawPipes();
Pipe* getList(int position);
bool isOffScreen(queue *q);
#endif
