#include "circleQueue.h"


QUEUE Queue; 
PQUEUE Q; 
/***********************************************
Function: Create a empty stack;
************************************************/
void createQueue(int maxsize)
{
	Q = &Queue;
	
	Q->pBase=(frame_t *)malloc(sizeof(frame_t)*maxsize);
	if(NULL==Q->pBase)
	{
		return; 
	}
	Q->front = 0; 
	Q->rear = 0;
	Q->maxsize = maxsize;
}

bool FullQueue()
{
	if(Q->front==(Q->rear+1)%Q->maxsize)
		return true;
	else
		return false;
}
bool EmptyQueue()
{
	if(Q->front==Q->rear) 
		return true;
	else
		return false;
}

bool Enqueue(frame_t *val)
{
	if(FullQueue(Q))
		return false;
	else
	{
		memcpy(&Q->pBase[Q->rear],val,sizeof(frame_t));
		
		Q->rear=(Q->rear+1)%Q->maxsize;
		return true;
	}
}

bool Dequeue(frame_t *val)
{
	if(EmptyQueue(Q))
	{
		return false;
	}
	else
	{
		memcpy(val,&Q->pBase[Q->front],sizeof(frame_t));
		
		Q->front=(Q->front+1)%Q->maxsize;
		return true;
	}
}
