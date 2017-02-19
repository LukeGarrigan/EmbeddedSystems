#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GUI.h"
#include <stdio.h>
#include <assert.h>
#include "main.h"
//Two glboal variables to store address of front and rear nodes. 



typedef struct queue *q;

queue * queueCreate(void)
{
    struct queue *q;

    q = malloc(sizeof(struct queue));

    q->head = q->tail = 0;

    return q;
}

/* push a new value onto top of Queue */
void enq(queue *q, Pipe *p)
{
    assert(p);


    /* Because I will be the tail, nobody is behind me */
    p->next = NULL;

    if(q->head == 0) {
        /* If the queue was empty, I become the head */
        q->head = p;
    } else {
        /* Otherwise I get in line after the old tail */
        q->tail->next = p;
    }

    /* I become the new tail */
    q->tail = p;
}

int queueEmpty(const struct queue *q)
{
    return (q->head == 0);
}

bool isOffScreen(queue *q){
	if(q->head->x < -50){
		return true;
	}else{
		return false;
	}
}

void deq(queue *q)
{
    int ret;
    Pipe *e;
	
    assert(!queueEmpty(q));
    /* patch out first element */
    e = q->head;
    q->head = e->next;	
	 // free(q);
}


/* free a queue and all of its elements */
void queueDestroy(struct queue *q)
{
    while(!queueEmpty(q)) {
        deq(q);
    }

    free(q);
}



void updatePipes(queue *q){
	Pipe* e;
	for(e = q->head; e != 0; e = e->next){
			e->x -= e->speed;
		}
}

/*
Pipe* initPipes(){
	int ySize = 100;
	int randTop= rand() % ySize;
	int randBot = rand() % ySize+100;
	Pipe *pipe;
	pipe = malloc(sizeof(Pipe));
	pipe->top =randTop;
	pipe->bottom = randBot;
	pipe->x = 600;
	pipe->speed = 5;
	return pipe;
}
*/


