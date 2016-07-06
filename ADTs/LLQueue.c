#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 0xffffffff


typedef enum{
	WHITE = 0,
	GREY = 1,
	BLACK
}COLOR;

typedef struct Vertex{
	Vertex *parent;
	int distance;
	int value;
	enum COLOR color;
}Vertex;


typedef struct Graph{
	Vertex **vs;
	int vcount;
}Graph;

typedef struct Queue{
	Vertex *front;
	Vertex *back;
	int count;
}Queue;

