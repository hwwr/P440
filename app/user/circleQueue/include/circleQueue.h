#ifndef __CIRCLEQUEUE_H_
#define __CIRCLEQUEUE_H_

#include <stdio.h>
#include <string.h>
#include "esp_common.h" 
#include "../../../include/user_config.h" 

typedef struct queue 
{
	frame_t *pBase;
	int front;    //指向队列第一个元素
	int rear;    //指向队列最后一个元素的下一个元素
	int maxsize; //循环队列的最大存储空间
}QUEUE,*PQUEUE;

void createQueue(int maxsize);
bool FullQueue();
bool EmptyQueue();
bool Enqueue(frame_t *val);
bool Dequeue(frame_t *val);
#endif