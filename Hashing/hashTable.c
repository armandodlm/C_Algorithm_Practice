#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#define MAX_STR_LEN 256
#define MAX_VAL_LEN 64
#define INITIAL_HASH_SIZE 103



typedef union{
	char st[MAX_STR_LEN];
	uint32_t vals[MAX_VAL_LEN];
}ctoint;

typedef struct Node{
	struct Node *previous;
	struct Node *next;
	char *key;
	char *value;
}Node;

typedef struct HashTable{
	unsigned int maxSize;
	unsigned int count;
	Node **values;
}HashTable;


unsigned int prehash(char *strKey);

// Hash equations
uint32_t divideHash(unsigned int key, unsigned int maxSize);
uint32_t multiplyHash(unsigned int key, unsigned int maxSize);
uint32_t universalHash(unsigned int key, unsigned int maxSize);

// Dictionary ADT operations
Node *findKey(HashTable *table,char *strKey);
void addKey(HashTable *table, char *key, char *value);
void grow(HashTable *table);
void rehash(HashTable *table, char *key, char *value);
void deleteKey(HashTable *table, char *strKey);
void shrink(HashTable *table);

void insertNode(Node *array, Node *element);
Node *initNode(char *key, char *value);
HashTable *initTable();

void destroyTable(HashTable *table);
void destroyNode(Node *n);
void destroyChain(Node *chain);
void destroyHashArray(Node **hashArray);

void printAllPairs(HashTable *theTable);

HashTable *initTable(uint32_t size){
	HashTable *table;
	table = (HashTable *)malloc(sizeof(HashTable));
	table->count = 1;
	table->maxSize = size;
	table->values = (Node **)calloc(sizeof(Node *),table->maxSize);
	return table;
}

Node *initNode(char *key, char *content){
	Node *n = (Node *)malloc(sizeof(Node));
	n->value = strdup(content);
	n->key = strdup(key);
	n->previous = NULL;
	n->next = NULL;
	return n;
}

void insertNode(Node *array, Node *element){
	if(array){
		Node *temp = array;
		while(temp->next != NULL){
			temp = temp->next;
		}
		//printf("Inserting node with key %s after %s \n",element->key,temp->key);
		temp->next = initNode(element->key,element->value);
		temp->next->previous = temp;
		temp->next->next = NULL;
	}else{
		//printf("First element in the list\n");
		array = initNode(element->key,element->value);
		array->next = NULL;
		array->previous = NULL;
	}
}

Node *findNode(Node *array,char *key){
	if(array){
		if(!strncmp(array->key,key,strlen(key))){
			return array;
		}else return findNode(array->next,key);
	}else return NULL;
}

// Can replace with post-order traversal (to destroy a binary s t)
void destroyChain(Node *chain){
	if(chain){
		Node *next = chain->next;
		if(next) destroyChain(next);		
		destroyNode(chain);
	}
}

// Assume that we don't have to worry about next and previous
void destroyNode(Node *n){
	if(n){
		if(n->value) free(n->value);
		if(n->key) free(n->key);
		free(n);
	}
}

void destroyTable(HashTable *table){
	if(table){
		uint32_t i = 0;
		for(i = 0; i < table->maxSize; i++){
			Node *val = table->values[i];
			destroyChain(val);
		}
		// Here we have destroyed all the nodes
	}
}


void grow(HashTable *table){

	if(table){
		printf("Growing from size %02d to size %02d\n",table->maxSize,table->maxSize*2);

		HashTable *temp = initTable(table->maxSize);
		temp->values = table->values;
		temp->count = table->count;

		table->maxSize = table->maxSize*2;
		table->values = (Node **)malloc(sizeof(Node *)*table->maxSize);
		table->count = 0;

		for(int i = 0; i < temp->maxSize; i++){
			Node *n = temp->values[i];
			while(n){
				rehash(table,n->key,n->value);
				n = n->next;
			}
			if(temp->values[i]) destroyChain(temp->values[i]);
		}

		/*
		HashTable temp = *table;
		table->maxSize = table->maxSize * 2;
		// Here double the size of the array
		table->values = (Node **)malloc(sizeof(Node *)*(table->maxSize));
		
		uint32_t i = 0;
		
		for(i = 0; i < temp.maxSize; i++){
			Node *no = temp.values[i];
			if(no){
				while(no){
					printf("REHASHING %s\n",no->key);
					rehash(table,no->key,no->value);	
					no = no->next;
				}
			} 
		}
		
		for(i = 0; i < temp.maxSize; i++){
			if(temp.values[i]){
				destroyChain(temp.values[i]);
			}
		}
		*/
		
	}
}

void addKey(HashTable *table, char *key, char *value){
	//printf("Adding %s : %s to the table\n",key,value);
	if(table){
		
		table->count += 1;
		if(table->count >= (table->maxSize)/2){
			printf("After adding this one count is %08x\n",table->count);
			grow(table);
		}
		uint32_t k = prehash(key);
		if(table->values[k % table->maxSize]){
			Node *n = initNode(key,value);
			//printf("Inserting at address: %08x\n",k%table->maxSize);
		//	printf("Collision detected!\n");
			insertNode(table->values[k % table->maxSize],n);
		}else{
			//printf("Inserting at address: %08x\n",k%table->maxSize);
			table->values[k % table->maxSize] = initNode(key,value);
		}
		
	}else{
		table = initTable(INITIAL_HASH_SIZE);
		uint32_t k = prehash(key);
		printf("Inserting at address: %08x\n",k%table->maxSize);
		table->values[k % table->maxSize] = initNode(key,value);
	}

}

// Assume the table exists
void rehash(HashTable *table, char *key, char *value){
	/*
	if(table){
		// If the table exists, then add a new node to the given key
		uint32_t k = prehash(key);
		Node *newNode = initNode(key,value);
		printf("prehash %08x, max size: %08x\n",k,table->maxSize);
		printf("Storing at index %02d \n",k%table->maxSize);
		insertNode(table->values[k % table->maxSize],newNode);
		table->count++;
	}
	*/
	//printf("Adding %s : %s to the table\n",key,value);
	if(table){
		
		table->count += 1;
		uint32_t k = prehash(key);
		if(table->values[k % table->maxSize]){
			Node *n = initNode(key,value);
			//printf("Inserting at address: %08x\n",k%table->maxSize);
			//printf("Collision detected!\n");
			insertNode(table->values[k % table->maxSize],n);
		}else{
			//printf("Inserting at address: %08x\n",k%table->maxSize);
			table->values[k % table->maxSize] = initNode(key,value);
		}	
	}
}



void shrink(HashTable *table){
	if(table){
		printf("Shrinking from size %02d to size %02d\n",table->maxSize,(int)floor(table->maxSize/2));
		//Node *temp = table->values;
		//HashTable temp = *table;
		
		// table contains  a ptr to the old table
		// tvals contains a ptr to the node array in the old table
		// tmax is the max # of vals in the old table

		//Node ** tvals = table->values;
		//uint32_t tmax = table->maxSize;

		HashTable *theTable = initTable(table->maxSize);
		theTable->values = table->values;
/*
		printf("Printing all in old table\n");
		printAllPairs(table);
		printf("Printing all in new table\n");
		printAllPairs(theTable);
*/
		// Add clamp or something
		table->maxSize = floor(table->maxSize/2);
		table->count = 0;
		table->values = (Node **)malloc(sizeof(Node *)*table->maxSize);
		/*
		printf("Address of old val : %p new val %p\n",tvals,table->values);
		uint32_t i = 0;

		printf("Printing the values of the newly allocated table\n");
		printAllPairs(table);
		printf("The next table should mimick the first two\n");
		printAllPairs(theTable);

		printf("The old table's max size was: %08x\n",theTable->maxSize);
		*/
		for(int i = 0; i < theTable->maxSize; i++){
			//printf("Looking at index %02d\n",i);
			Node *n = theTable->values[i];
			while(n){
				//printf("Looking at element -> %s \n",n->key);
				rehash(table,n->key,n->value);
				n = n->next;
			}
			if(theTable->values[i]) destroyChain(theTable->values[i]);
		}

	//	printf("Done with rehashing everything, now showing the new table with n = %02d and m = %08x\n",table->count,table->maxSize);
		//printAllPairs(table);
/*
		// Make sure things are accessible in order
		printf("At shrinking point@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		printAllPairs(theTable);

		for(int i = 0; i < theTable->maxSize; i++){
			Node *n = theTable->values[i];
			uint32_t s = 0;
			while(n){
				//printf("There is an element in the talbe with key %s and value %s stored at index %02d in a chain of size %02d \n",n->key,n->value,i,s);
				//s++;

				printf("REHASHING\n");
				rehash(table,n->key,n->value);
				n = n->next;
			}
		}
		*/
/*
		for(i = 0; i < tmax; i++){
			Node *no = tvals[i];
			printf("At index %02d ----------- in the original talbe \n",i);
			if(no){
				do{
					printf("REHASHING %s\n",no->key);
					rehash(table,no->key,no->value);
					no = no->next;
				}while(no);
			}
		}

		for(i = 0; i < tmax; i++){
			if(tvals[i]){
				destroyChain(tvals[i]);
			}
		}
	*/
	}
}


Node *findKey(HashTable *table,char *key){
	uint32_t k = prehash(key);
	//printf("The key is : %s %08x\n",key,k);
	//printf("Looking at address : %08x\n",k % table->maxSize);
	return findNode(table->values[k % table->maxSize],key);
}

void deleteKey(HashTable *table,char *strKey){
	//printf("~~~~~~~~~~~~DELETING KEY~~~~~~~~~~\n");
	//Node *dnode = findKey(table,strKey);

	uint32_t k = prehash(strKey);
	Node *dnode = findNode(table->values[k % table->maxSize],strKey);

	if(dnode){
		//printf("Found a node with : %s : %s \n",dnode->key,dnode->value);
		if(dnode->previous){
			//printf("It had a previous value\n");
			if(dnode->next){
			//	printf("It had a next value\n");
				Node *p = dnode->previous;
				p->next = dnode->next;
				p->next->previous = p;
			}else{
			//	printf("It didn't have a next value\n");
				Node *p = dnode->previous;
				p->next = NULL;
			}
			destroyNode(dnode);
		}else{
			//printf("It was the first in the chain\n");
			free(dnode->value);
			free(dnode->key);
			if(dnode->next){
				table->values[k % table->maxSize] = dnode->next;
			}else{
				table->values[k % table->maxSize] = NULL;
			}
		}
		table->count--;
		if(table->count <= (table->maxSize/4)){
			shrink(table);
		}
	}
}


uint32_t divideHash(unsigned int key, unsigned int maxSize){
	
	return (uint32_t)(key % maxSize);
}
uint32_t multiplyHash(unsigned int key, unsigned int p){
	/*
	uint32_t s = A << 32;
	uint64_t ks = key * s;
	uint32_t r1 = (ks) >> 32;
	uint32_t r0 = (ks) - r1;

	return r0 >> (32 - p);
	*/
	return 0;
}

uint32_t universalHash(unsigned int key, unsigned int maxSize){

	return 0;
}



uint32_t prehash(char *str){

	ctoint x;
	int len = strlen(str);
	int numblocks = (len % 4 ? (len>>2) + 1: len>>2);
	
	//printf("There are %02d letters, which translate to %02d blocks\n",len,numblocks);
	
	memset(x.st,0,MAX_STR_LEN);
	strncpy(x.st,str,numblocks*4);
	
	int iter = 0;
	uint32_t tsum = 0;
	for(iter = 0; iter < numblocks; iter++){
		tsum += x.vals[iter];
	}

	return tsum;
}

uint32_t prehash2(char *str){
	int len = strlen(str);
	numBlocks = (len % 4 ? len/4 + 1 : len/4);
	char *iter = str;
	uint32_t prehash = 0x00;
	while(iter != '\0'){

	}
}

void printAllPairs(HashTable *theTable){
	printf("Max elements : %08x\nNum elements : %08x\n",theTable->maxSize,theTable->count);
	for(int i = 0; i < theTable->maxSize; i++){
		Node *n = theTable->values[i];
		uint32_t s = 0;
		while(n){
			printf("There is an element in the talbe with key %s and value %s stored at index %02d in a chain of size %02d \n",n->key,n->value,i,s);
			s++;
			n = n->next;
		}
	}
}


int main(){

	HashTable *theTable = initTable(INITIAL_HASH_SIZE);
	addKey(theTable,"apple","pie");
	addKey(theTable,"orange","cider");
	addKey(theTable,"pikachu","rules");
	addKey(theTable,"en","un");
	addKey(theTable,"lugar de","la mancha");
	addKey(theTable,"de curyo","nombre no");
	addKey(theTable,"quiero","acordarme");
	addKey(theTable,"no hace","mucho que");
	addKey(theTable,"vivia un","hombre de");
	addKey(theTable,"rocin","flaco");
	addKey(theTable,"adarga antigua","y galgo corredor");
	addKey(theTable,"un sopin","mas que");
	addKey(theTable,"aaaabbbb","burp");
	addKey(theTable,"bbbbaaaa","meh");

	//uint32_t x = theTable->count;

	printAllPairs(theTable);


	deleteKey(theTable,"pikachu");
	deleteKey(theTable,"vivia un");
	deleteKey(theTable,"aaaabbbb");
	deleteKey(theTable,"en");

	printAllPairs(theTable);

	addKey(theTable,"animal","acordarme");
	addKey(theTable,"artic","mucho que");
	addKey(theTable,"antartic","hombre de");
	addKey(theTable,"metropolitan","flaco");
	addKey(theTable,"antimetro","y galgo corredor");
	addKey(theTable,"lumbersexual","mas que");
	addKey(theTable,"metrosexual","burp");
	addKey(theTable,"albaricoque","meh");
	addKey(theTable,"melocoton","acordarme");
	addKey(theTable,"ipad","mucho que");
	addKey(theTable,"ipodipod","hombre de");
	addKey(theTable,"ipad","flaco");
	addKey(theTable,"mallulah","y galgo corredor");
	addKey(theTable,"flashcard","mas que");
	addKey(theTable,"marker","burp");
	addKey(theTable,"whiteboard","meh");


	printAllPairs(theTable);

/*
	k = findKey(theTable,"pikachu");
	if(k) printf("pikachu: %s\n",k->value);
	else printf("Could not find the value\n");

	k = findKey(theTable,"lugar de");
	if(k) printf("lugar de: %s\n",k->value);
	else printf("Could not find the value\n");
	k = findKey(theTable,"de cuyo");
	if(k) printf("de cuyo: %s\n",k->value);
	else printf("Could not find the value\n");
	k = findKey(theTable,"quiero");
	if(k) printf("quiero : %s\n",k->value);
	else printf("Could not find the value\n");
	k = findKey(theTable,"no hace");
	if(k) printf("no hace: %s\n",k->value);
	else printf("Could not find the value\n");
*/

	destroyTable(theTable);

	return 0;
}

