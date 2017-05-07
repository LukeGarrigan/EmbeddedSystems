#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GUI.h"
#include <stdio.h>
#include <assert.h>
#include "main.h"
//Two glboal variables to store address of front and rear nodes. 



typedef struct queue *q;

/** @defgroup Queue_Structure
* @{
*/ 

/**
 * @brief Creates empty queue structure
 * @param  None
 * @retval queue
 *
 */
queue * queueCreate(void){
    struct queue *q;
	
    q = malloc(sizeof(struct queue));
    q->head = q->tail = 0;

    return q;
}

/**
 * @brief Push pipe to queue
 * @param  queue *q, Pipe *p
 * @retval None
 *
 *
 * Method pushes given pipe to the top of a given queue
 *
 */
void enq(queue *q, Pipe *p)
{
    //assert(p);
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

/**
 * @brief Checks if the queue is empty
 * @param  None
 * @retval None
 *
 */
int queueEmpty(const struct queue *q)
{
    return (q->head == 0);
}


/**
 * @brief Checks if the queue is off screen
 * @param  queue *q
 * @retval bool
 *
 */
bool isOffScreen(queue *q){
	if(q->head->x < -50){
		return true;
	}else{
		return false;
	}
}


/**
 * @brief Removes pipe at the head of the queue
 * @param  queue *q
 * @retval None
 *
 */
void deq(queue *q)
{
    int ret;
    Pipe *e;
	
    //assert(!queueEmpty(q));
    /* patch out first element */
    e = q->head;
    q->head = e->next;	
	  free(e);
}



/**
 * @brief Removes entire queue and frees memory
 * @param  queue *q
 * @retval None
 *
 */
void queueDestroy(struct queue *q)
{
    while(!queueEmpty(q)) {
        deq(q);
    }
		free(q);
}

/**
 * @brief Updates pipe positions
 * @param  queue *q
 * @retval None
 *
 */
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


