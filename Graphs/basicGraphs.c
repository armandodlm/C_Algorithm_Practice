#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 0xffffffff


typedef enum{
	WHITE = 0,
	GREY = 1,
	BLACK
}COLOR;

typedef struct Edge{
	Edge *next;
	Vertex *node;
}Edge;

typedef struct Vertex{
	Vertex *next;
	Edge *edges;
	int isvisited;
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



Vertex *initVertex(int value){
	Vertex *v = (Vertex *)malloc(sizeof(Vertex));
	v->edges = NULL;
	v->distance = INFINITY;
	v->parent = NULL;
	v->value = value;
	v->color = WHITE;
}

void destroyVertex(Vertex *v){

}

