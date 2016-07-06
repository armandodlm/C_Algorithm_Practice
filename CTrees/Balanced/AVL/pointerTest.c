#include <stdio.h>
#include <stdlib.h>


void switchOne(int *p, int *p2);
void switchTwo(int **p, int *p2);
void switchThree(int *p, int *p2);

void switchOne(int *p, int *p2){
	p = p2;
}

void switchTwo(int **p, int *p2){
	*p = p2;
}

void switchThree(int *p, int *p2){
	*(&p) = p2;
	
}

int main(){
	int *p = malloc(sizeof(int));
	int *p2 = malloc(sizeof(int));

	int *temp1 = p;
	int *temp2 = p2;

	int **pp;

	printf("p points to -> %p \n",p);
	printf("p2 points to -> %p \n",p2);

	switchOne(p,p2);
	printf("After s1 \n");
	printf("p points to -> %p \n",p);
	printf("p2 points to -> %p \n",p2);
	
	p = temp1;
	p2 = temp2;

	switchTwo(&p,p2);
	printf("After s2 \n");
	printf("p points to -> %p \n",p);
	printf("p2 points to -> %p \n",p2);

	p = temp1;
	p2 = temp2;

	switchThree(p,p2);
	printf("After s3 \n");
	printf("p points to -> %p \n",p);
	printf("p2 points to -> %p \n",p2);

	free(p);
	free(p2);
}