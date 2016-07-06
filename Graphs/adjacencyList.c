#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"

typedef struct Vertex{
	struct Vertex *adjacent;
	struct BaseVertex *arrayVertex;
}Vertex;

typedef struct BaseVertex{
	struct Vertex *adjacent;
	int vertexValue;
	int distance;
	int parent;
	int color;
}BaseVertex;

typedef struct Graph{
	//Vertex **lists;
	BaseVertex *vertices;
	int vertexCount;
}Graph;

typedef enum{
	WHITE = 0,
	GREY = 1,
	BLACK = 2
}COLOR;

void addEdge(Graph *graph, int source, int destination);

Vertex *initVertex(BaseVertex *base);
Graph *initGraph(int v);

void destroyVertexChain(Vertex *v);
void destroyGraph(Graph *g);


void BFS(Graph *graph, int s);
void DFS(Graph *graph, int s);

void printGraph(Graph *graph);

Graph *initGraph(int v){
	Graph *graph = (Graph *)malloc(sizeof(Graph));
	graph->vertexCount = v;
	graph->vertices = (BaseVertex *)malloc(sizeof(BaseVertex)*v);

	int c;
	// Meaning index c does not point to anything yet
	for(c = 0; c < v; c++){
		graph->vertices[c].adjacent = NULL;
		graph->vertices[c].distance = 0;
		graph->vertices[c].parent = -1;
		graph->vertices[c].color = WHITE;
		graph->vertices[c].vertexValue = c;
	}

	return graph;
}

void destroyVertexChain(Vertex *v){
	if(v){
		Vertex *temp = v;
		Vertex *next;
		while(temp){
			next = temp->adjacent;
			free(temp);
			temp = next;
		}
	}
}

void destroyGraph(Graph *g){
	if(g){
		int c;
		for(c = 0; c < g->vertexCount; c++){
			destroyVertexChain(g->vertices[c].adjacent);
		}
		free(g);
	}
}

Vertex *initVertex(BaseVertex *base){
	Vertex *ver = (Vertex *)malloc(sizeof(Vertex));
	ver->adjacent = NULL;
	ver->arrayVertex = base;
	return ver;
}

void addEdge(Graph *graph, int source, int destination){
	if(graph){

		BaseVertex *b = graph->vertices+source;

		Vertex *ad = b->adjacent;
		b->adjacent = initVertex(graph->vertices+destination);
		b->adjacent->adjacent = ad;
		/*
		Vertex *d = initVertex(graph->vertices+destination);
		d->adjacent = graph->vertices+source;
		graph->lists[source] = d;

		Vertex *s = initVertex(source,0);
		s->adjacent = graph->lists[destination];
		// if unidirectional add another one for the destination
		graph->lists[destination] = s;
		*/
	}
}


void printGraph(Graph *graph){
	if(graph){
		int c = 0;
		for(c = 0; c < graph->vertexCount; c++){
			Vertex *node = graph->vertices[c].adjacent;
			printf(" %d ->",c);
			while(node){
				printf(" %d ->",node->arrayVertex->vertexValue);
				node = node->adjacent;
			}
			printf("NULL\n");
		}
	}else{
		printf("The graph is empty\n");
	}
}


void printVertexStatistics(Graph *graph){
	int c;
	for(c = 0; c < graph->vertexCount; c++){
		printf("----- This is node %02d -----\n",c);
		printf("Color : %02d\nParent : %02d\nDistance : %02d\n",graph->vertices[c].color,graph->vertices[c].parent,graph->vertices[c].distance);
	}
}



void BFS(Graph *graph, int s){
	Queue *queue = initQueue(graph->vertexCount);
	int index;
	printf("Beginning BFS from node %02d\n",s);
	printf("Initial stats:\n");
	printVertexStatistics(graph);

	BaseVertex *v = graph->vertices+s;
	v->color = GREY;
	v->distance = 0;
	v->parent = -1;

	// Assume the nodes are initialized here 	
	enqueue(queue,s);

	while(queue->count != 0){
		index = dequeue(queue);
		// while there are adjacent nodes
		v = graph->vertices+index;
		Vertex *adjacent = v->adjacent;

		while(adjacent){
			// if the vertex to which we can get from this one vertex is white (not been modified yet)
			if(adjacent->arrayVertex->color == WHITE){
				adjacent->arrayVertex->distance = graph->vertices[index].distance + 1;
				adjacent->arrayVertex->parent = index;
				adjacent->arrayVertex->color = GREY;
				enqueue(queue,adjacent->arrayVertex->vertexValue);
			}
			adjacent = adjacent->adjacent;
		}
	}

	// At this point all nodes should've been accessed breadth first
	printVertexStatistics(graph);

	destroyQueue(queue);
}

int main(){
	Graph *graph = initGraph(10);

	addEdge(graph,1,2);
	addEdge(graph,1,0);
	addEdge(graph,2,0);
	addEdge(graph,2,3);
	addEdge(graph,2,1);
	addEdge(graph,2,4);
	addEdge(graph,0,1);
	addEdge(graph,0,2);
	addEdge(graph,0,3);
	addEdge(graph,0,5);
	addEdge(graph,5,0);
	addEdge(graph,5,3);
	addEdge(graph,3,0);
	addEdge(graph,3,5);
	addEdge(graph,3,2);
	addEdge(graph,3,4);
	addEdge(graph,4,3);
	addEdge(graph,4,2);

	printGraph(graph);

	BFS(graph,2);

	destroyGraph(graph);
}
