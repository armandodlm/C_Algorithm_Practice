#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LEFT(i) ((i)*(2) + 1)
#define RIGHT(i) (((i)*(2)) + (2) )

typedef struct{
	int *heap;
	unsigned int size;
}heap;


void max_heapify(heap *theHeap, int index);
void printArray(int *array, int size);
// presuposes that h contains an array and a size of said array, this array is assumed to not hold the heap property
void build_heap(heap *h);
heap *initializeHeap(int *array, int size);
void deleteHeap(heap *h);
heap *heapSort(int *array, int size);


// Assume the heap has been initialized and all its attributes are in the struct
void max_heapify(heap *h, int index){
	int left = LEFT(index);
	int right = RIGHT(index);
	int max = index;

	//printf("Comparing indexes: %d  %d and %d \n",index,left,right);
	if(left < h->size && h->heap[left] > h->heap[index]){
		max = left;
	}
	if(right < h->size && h->heap[right] > h->heap[max]){
		max = right;
	}
	if(max != index){
		// swap the values
		//printf("Exchanging %d and %d \n",h->heap[index],h->heap[max]);
		int temp = h->heap[index];
		h->heap[index] = h->heap[max];
		h->heap[max] = temp;
		//printArray(h->heap,10);
		max_heapify(h,max);
	}
}

void build_heap(heap *h){
	int half = floor(h->size/2);
	int index = 0;

	for(index = half; index >= 0; index--){
		max_heapify(h,index);
	}
}


void printArray(int *array, int size){
	int index = 0;
	printf("The array is: [");
	for(index = 0; index < size; index++){
		printf("%02d ",array[index]);
	}
	printf("]\n");
}

// Assume the array is initialized
heap *initializeHeap(int *array, int size){
	if(array != NULL && size > 0){
		heap *h = (heap *)malloc(sizeof(heap));
		h->heap = array;
		h->size = size;
		build_heap(h);

		return h;
	}else{
		return NULL;
	}
}

void deleteHeap(heap *h){
	if(h != NULL) free(h);
}


heap *heapSort(int *array, int size){
	heap *h = initializeHeap(array,size);
	int index = 0;
	//printf("~~~~~~~ Heap built ~~~~~~~\n");
	//printArray(h->heap,10);
	for(index = h->size-1; index >= 0; index--){
		int temp = h->heap[index];
		//printf("Exchanging %d (at %d) for %d\n",temp,index,h->heap[0]);
		h->heap[index] = h->heap[0];
		h->heap[0] = temp;
		h->size -= 1;
		max_heapify(h,0);
		//printf("Heap size gone to: %d\n",h->size);
		//printArray(h->heap,10);
	}

	return h;
}

int main(){
	int nums[10] = {30,28,26,24,22,20,18,29,27,25};

	int nums2[30] = {30,28,26,24,22,20,18,29,27,25,	
					 23,21,21,23,21, 1, 2, 3, 4, 5,
					 6 , 7, 8, 9,10,11,13,12,14,16};

	printArray(nums2,30);

	heap *h = heapSort(nums2,30);
	printArray(h->heap,30);

	deleteHeap(h);

	return 0;
}