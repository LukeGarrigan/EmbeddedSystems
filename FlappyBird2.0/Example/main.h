
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
#endif /* _MAIN_H */