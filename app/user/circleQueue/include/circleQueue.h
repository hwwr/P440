#ifndef __CIRCLEQUEUE_H_
#define __CIRCLEQUEUE_H_

#include <stdio.h>
#include <string.h>
#include "esp_common.h" 
#include "../../../include/user_config.h" 

typedef struct queue 
{
	frame_t *pBase;
	int front;    //ָ����е�һ��Ԫ��
	int rear;    //ָ��������һ��Ԫ�ص���һ��Ԫ��
	int maxsize; //ѭ�����е����洢�ռ�
}QUEUE,*PQUEUE;

void createQueue(int maxsize);
bool FullQueue();
bool EmptyQueue();
bool Enqueue(frame_t *val);
bool Dequeue(frame_t *val);
#endif