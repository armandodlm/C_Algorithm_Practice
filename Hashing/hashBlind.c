#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#include "BSTNode.h"

typedef struct Node{
	struct Node *left;
	struct Node *right; 
	struct Node *parent;
	uint32_t key;
	char *value;
}Node;

typedef struct HashTable{
	Node **table;
	uint32_t p;
	uint32_t maxSize;
	uint32_t count;
}HashTable;

Node *initNode(uint32_t key, char *value);
void deleteNode(Node *node);

HashTable *initHash(uint32_t maxSize);
void deleteHash(HashTable *table);

void addKey(HashTable *table, char *key, char *value);
Node *deleteKey(HashTable *table, char *key);
Node *findKey(HashTable *table, char *key);
uint32_t prehash(char *key);
uint32_t hashMultiplication(uint32_t key, uint32_t p);
uint32_t hashDivision(uint32_t key, uint32_t max);

void growHash(HashTable *table);
void shrinkHash(HashTable *table);

void dealocateTree(Node *root);

// BST functions
void BSTinsertNode(Node *root, Node *pair){
	if(root){
		if(pair->key > root->key){
			if(root->right){
				BSTinsertNode(root->right,pair->key);	
			} 
			else{
				// insert here
				root->right = pair;
				root->right->parent = root;
			}
		}else if(pair->key < root->key){
			if(root->left){
				BSTinsertNode(root->left,pair->key);
			}else{
				root->left = pair;
				root->left->parent = root;
			}
		}else{
			root->value = pair->value;
		}
	}else{
		root = pair;
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
	}
}



Node *initNode(uint32_t key, char *value){
	Node *n = (Node *)malloc(sizeof(Node));	
	n->left = NULL;
	n->right = NULL;
	n->key = key;
	n->value = strdup(value);
	return n;
}

void deallocateTree(Node *root){
	if(root){
		deallocateTree(root->left);
		deallocateTree(root->right);
		if(root->value) free(root->value);
		free(root);
	}
}

void deleteNode(Node *node){
	if(node){
		if(node->value) free(node->value);
		free(node);
	}
}

HashTable *initHash(uint32_t maxSize){
	HashTable *t = (HashTable *)malloc(sizeof(HashTable));
	t->table = (Node **)malloc(sizeof(Node *)*maxSize);
	t->p =5;
	t->maxSize = 1<<(t->p);
	t->count = 0;
	
	return t;
}	


void deleteHash(HashTable *table){
	if(table){
		int i = 0;
		for(i = 0; i < table->maxSize; i ++){
			if(table->table[i]){
				deallocateTree(table->table[i]);
			}
		}
		free(table);
	}
}

uint32_t prehash(char *key){
	
	char *iter = key;

	int len = strlen(key);
	int numBlocks = (len % 4 ? len/4 + 1 : len/4);

	int i = 0;
	uint32_t shif = 0;
	int extra = len % 4;

	for(i = 0; i < numBlocks; i++){
		if(i == numBlocks-1){
			int ex = 0;
			for(ex = 0; ex < extra; ex++){
				shif += *(iter+ex);
			}
		}else{
			shif |= *iter;
			shif |= (*(iter+1))<<8;
			shif |= (*(iter+2))<<16;
			shif |= (*(iter+3))<<24;
		}
		iter += 4;
	}
	return shif;
}

uint32_t hashDivision(uint32_t key,uint32_t max){
	return key % max;
}

uint32_t hashMultiplication(uint32_t key, uint32_t p){
	double A = (sqrt(5)-1)/2;
	printf("A %16f\n",A);
	uint64_t mw = 1<<32;
	uint64_t s = A * mw;
	uint64_t r = s * key;
	printf("r = %16x\n",r);
	uint64_t r1 = r >> 32;
	uint64_t r0 = (r) - (r << 32);
	return (uint32_t)(r0 >> (32 - p));
}

void addKey(HashTable *table, char *key, char *value){
	if(table){
		uint32_t phash = prehash(key);
		uint32_t hindex = hashDivision(phash,table->maxSize);
		if(table->table[hindex]){
			// append to BST
			Node *pair = initNode(phash,value);
			BSTinsertNode(pair);
			table->count++;
			if(table->count >= (table->maxSize/2)){
				growHash(table);
			}
		}else{
			table->table[hindex] = initNode(phash,value);
			table->count++;
		}
	}
}

// do in order traversal of tree 
// addKey for each node
void inorderAdd(HashTable *table,Node *parent){
	if(parent){
		inorderAdd(parent->left);
		addKey(table,parent->key,parent->value);
		inorderAdd(parent->right);
	}
}

void growHash(HashTable *ht){
	//HashTable *t = initHash(ht->maxSize);
	Node **temptable = ht->table;
	uint32_t maxCount = ht->maxSize;
	//t->table = 
	//t->count = ht->count;

	ht->maxSize = ht->maxSize*2;
	ht->table = (Node **)calloc(ht->maxSize,sizeof(Node *));

	uint32_t i = 0;
	for(i = 0; i < maxCount; i++){
		Node *n = temptable[i];
		// n is potentially the root of a bst
		inorderAdd(ht,n);
	}
}

Node *deleteKey(HashTable *table, char *key);
Node *findKey(HashTable *table, char *key);

void growHash(HashTable *table);
void shrinkHash(HashTable *table);


int main(){
	char *value = "once upon a time there was an apple\n";
	char *v2 = "television";
	printf("The prehash returns : %08x \n",prehash(value));
	printf("The prehash returns : %08x \n",prehash(v2));

	printf("Multi : %08x\n",hashMultiplication(prehash(value),5));
	printf("Multi : %08x\n",hashMultiplication(prehash(v2),5));
	printf("Divi : %08x\n",hashDivision(prehash(value),32));
	printf("Divi : %08x\n",hashDivision(prehash(v2),32));
	return 0;
}