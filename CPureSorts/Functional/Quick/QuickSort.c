#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void quicksort(int *array, int low, int high);
int partition(int *array, int low, int high);
void printArray(int *array, int size);


void quicksort(int *array, int low, int high){
	if(low < high){
		int p = partition(array,low,high);
		quicksort(array,low,p-1);
		quicksort(array,p+1,high);
	}
}

int partition(int *array, int low, int high){

	int left, right;
	int pivot = array[low];

	left = low;
	right = high;

	while(left < right){

		while(array[left] <= pivot){
			left++;
		}
		while(array[right] > pivot){
			right--;
		}
		if(left < right){
			//swap as the two elements being pointed to are not in the right location
			int temp = array[left];
			array[left] = array[right];
			array[right] = temp;
		}
	}
	// exiting means that left right met somewhere
	// swap the pivot with the right (if left right are equal it shouldn't matter)
	array[low] = array[right];
	array[right] = pivot;
	return right;
}

void printArray(int *array, int size){
	int index = 0;
	printf("The array is: [");
	for(index = 0; index < size; index++){
		printf("%02d ",array[index]);
	}
	printf("]\n");
}

int main(){

	//int nums[10] = {30,28,26,24,22,20,18,29,27,25};
	int nums[10] = {10,30,20,40,50,60,70,80,100,90};
	int nums2[30] = {30,28,26,24,22,20,18,29,27,25,	
					 23,21,19,17,15, 1, 2, 3, 4, 5,
					 6 , 7, 8, 9,10,11,13,12,14,16};

	printArray(nums,10);
	quicksort(nums,0,9);
	printArray(nums,10);

	printArray(nums2,30);
	quicksort(nums2,0,29);
	printArray(nums2,30);


	return 0;
}

