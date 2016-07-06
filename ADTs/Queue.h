#ifndef QUEUE_H
#define QUEUE_H

#define ERROR 999999

typedef struct Queue{
	int front;
	int back;
	int count;
	int maxSize;
	int *queue;
}Queue;

int enqueue(Queue *q, int value);
int dequeue(Queue *q);

Queue *initQueue(int size);
void destroyQueue(Queue *queue);

void printQueue(Queue *queue);
void showStatus(Queue *queue);

#endif
