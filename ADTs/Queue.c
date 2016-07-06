#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Queue.h"

Queue *initQueue(int size){
	Queue *q = (Queue *)malloc(sizeof(Queue));
	q->queue = (int *)calloc(size,sizeof(int));
	q->front = 0;
	q->back = 0;
	q->count = 0;
	q->maxSize = size;
	return q;
}

void destroyQueue(Queue *queue){
	free(queue->queue);
	free(queue);
}

int enqueue(Queue *queue, int value){
	if(queue){
		if(queue->count < queue->maxSize){
			//printf("Trying to add element at index %d \n",queue->back);
			queue->queue[queue->back] = value;
			queue->back++;
			if(queue->back == queue->maxSize) queue->back = 0;

			queue->count++;
			return 0;
		}else return -1;
	}else return -1;
}

int dequeue(Queue *queue){
	if(queue){
		if(queue->count > 0){
			int value = queue->queue[queue->front];
			queue->count--;
			queue->front++;
			if(queue->front == queue->maxSize) queue->front = 0;
			return value;
		}else return ERROR;
	}else return ERROR;
}


void printQueue(Queue *queue){
	if(queue){
		if(queue->count > 0){
			int i = 0;
			printf("Head of the queue is at %d \n",queue->front);
			printf("Back of the queue is at %d \n",queue->back);
			printf("[");
			if(queue->front == queue->back){
				printf(" %d ",queue->queue[queue->front]);	
			}else{
				while(queue->front != queue->back){
					printf(" %d ",queue->queue[queue->front]);
					queue->front++;
					if(queue->front == queue->maxSize ) queue->front = 0;
				}		
				printf(" %d ",queue->queue[queue->front]);
			}
			printf("]\n");
		}else printf("The queue is empty\n");
	}else printf("The queue has not been initialized\n");
}


void showStatus(Queue *queue){
	int last = queue->back;
	last--;
	if(last < 0) last = queue->maxSize-1;
	printf("The front is at index %d and the back is at index %d\n",queue->front,queue->back);
	printf("Head value is : %d\n",queue->queue[queue->front]);
	printf("Back value is : %d\n",queue->queue[last]);
}	


int main(){
	Queue *queue = initQueue(20);

	enqueue(queue,50);
	showStatus(queue);
	enqueue(queue,10);
	showStatus(queue);
	enqueue(queue,60);
	showStatus(queue);
	dequeue(queue);
	showStatus(queue);
	enqueue(queue,80);
	showStatus(queue);
	dequeue(queue);
	showStatus(queue);



	free(queue->queue);
	free(queue);
	return 0;
}


