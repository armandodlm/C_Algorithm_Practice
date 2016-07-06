#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_STR_LEN 256
#define MAX_VAL_LEN 64

typedef union{
	char st[MAX_STR_LEN];
	uint32_t vals[MAX_VAL_LEN];
}ctoint;

uint32_t prehashString(char *str);

uint32_t prehashString(char *str){
	
	ctoint x;
	int len = strlen(str);
	int numblocks = (len % 4 ? (len>>2) + 1: len>>2);
	
	printf("There are %02d letters, which translate to %02d blocks\n",len,numblocks);
	
	memset(x.st,0,MAX_STR_LEN);
	strncpy(x.st,str,numblocks*4);
	
	int iter = 0;
	uint32_t tsum = 0;
	for(iter = 0; iter < numblocks; iter++){
		tsum += x.vals[iter];
	}

	return tsum;
}


int main(){
	char *s1 = (char *)malloc(sizeof(20));
	strncpy(s1,"abcdefg",7);
	char *s2 = (char *)malloc(sizeof(30));
	strncpy(s2,"abcdefg",7);
	char *s3 = "abcde";
	char s4[6];

	printf("Size of s1 %02d\n",strlen(s1));
	printf("Size of s2 %02d\n",strlen(s2));
	printf("Size of s3 %02d\n",strlen(s3));
	printf("Size of s4 %02d\n",strlen(s4));


	char *s5 = strdup("This is a string\n");
	char *s6 = strdup("This is another string\n");
	char *s7 = strdup("And this is our last string\n");
	char *s8 = strdup("This is a string\n");


	printf("%s",s5);
	printf("Size of s5 %02d\n",strlen(s5));
	printf("Size of s6 %02d\n",strlen(s6));
	printf("Size of s7 %02d\n",strlen(s7));

	uint32_t tsum = prehashString(s5);
	printf("s5 has a prehash value of %08x \n",tsum);
	tsum = prehashString(s6);
	printf("s6 has a prehash value of %08x \n",tsum);
	tsum = prehashString(s7);
	printf("s7 has a prehash value of %08x \n",tsum);
	tsum = prehashString(s8);
	printf("s8 has a prehash value of %08x \n",tsum);


	
	free(s1);
	free(s2);

	return 0;
}