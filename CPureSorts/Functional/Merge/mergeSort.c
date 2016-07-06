#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void mergeSort(char *array, unsigned int low, unsigned int high);
void merge(char *array, unsigned int low, unsigned int middle, unsigned int high);
void printArray(char *array, int size);

// Takes in the unsorted array and the size of said array
// Returns the sorted array of numbers
void mergeSort(char *array, unsigned int low, unsigned int high){
	if( low < high ){
		unsigned int midpoint = low + floor((high-low)/ 2);
		//printf("low = %02x, high = %02x\n",low,high);
		//printf("midpoint = %002x\n",midpoint);
		mergeSort(array,low,midpoint);
		mergeSort(array,midpoint+1,high);
		merge(array,low,midpoint,high);
	}else{// otherwise the array is at most of size 1, so it's sorted
		//printf("Attempted a sort, but low was %02x and high was %02x\n",low,high);
	}
}



void merge(char *array, unsigned int low, unsigned int middle, unsigned int high){
	//printf("Merging with low = %02x, middle = %02x, high = %02x\n",low,middle,high);
	unsigned int sizeleft, sizeright, index, leftindex, rightindex;
	sizeleft = (middle - low ) + 1;
	sizeright = (high - middle);
	//printf("Size of left: %02x and size of right: %02x\n",sizeleft,sizeright);
	char left[sizeleft];
	char right[sizeright];

	// Sinze low  could be any index in the array, we use it as a base index. 
	// Getting anything from the array should be found using that base index
	for(index = 0; index < sizeleft; index++){
		//printf("Accessing index %02x from original. Assigning to index %02x of left\n",(low+index),index);
		left[index] = array[low + index];
	}
	for(index = 0; index < sizeright; index++){
		//printf("Accessing index %02x from original. Assigning to index %02x of right\n",(middle+ 1 + index),index);
		right[index] = array[middle + 1 + index];
	}

	// Now we have two arrays we need to merge
	// Start drawing from "the top of the decks"
	index = 0;
	leftindex = 0;
	rightindex = 0;
	//printf("Merging the following arrays:\n");
	//printArray(left,sizeleft);
	//printArray(right,sizeright);

	while(leftindex < sizeleft && rightindex < sizeright){
		//printf("Out of %02x and %02x ...\n",left[leftindex],right[rightindex]);
		if(left[leftindex] <= right[rightindex]){
			array[low + index] = left[leftindex];
			leftindex++; // we have placed one from the left
		}else{
			array[low + index] = right[rightindex];
			rightindex++; // We have placed one from the right
		}
		//printf("The lowest was: %02x\n",array[index]);
		index++;
	}

	while(leftindex < sizeleft){
		array[low + index] = left[leftindex];
		leftindex++;
		index++;
	}

	while(rightindex < sizeright){
		array[low + index] = right[rightindex];
		rightindex++;
		index++;
	}
	//printArray(array,30);
	//printArray(array,high-low+1);

}



void printArray(char *array, int size){
	int index = 0;
	printf("The array is: [");
	for(index = 0; index < size; index++){
		printf("%02x ",array[index]);
	}
	printf("]\n");
}

int main(){
	char nums[30] = {30,28,26,24,22,20,18,29,27,25,
					 23,21,19,17,15, 1, 2, 3, 4, 5,
					 6 , 7, 8, 9,10,11,13,12,14,16};


	printArray(nums,30);
	mergeSort(nums,0,29);
	printArray(nums,30);

}