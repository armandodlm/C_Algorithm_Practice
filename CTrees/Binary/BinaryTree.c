#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Node{
	struct Node *parent;
	struct Node *left;
	struct Node *right;
	int key;
}Node;

Node *initNode(Node *parent,Node *left, Node *right, int key);
void insertValue(Node *tree, int key);
void inOrderWalk(Node *root);
void printTree(Node *root);
void deallocateTree(Node *root);
void deleteNode(Node *root, int key);
Node *findMin(Node *root);
Node *findNode(Node *root,int key);
Node *findMax(Node *root);

void insertValue(Node *tree, int key){
	if(tree == NULL){
		tree = initNode(NULL,NULL,NULL,key);
	}else{
		Node *iter = tree;
		//Node *parent = NULL;
		while(iter != NULL){
			//parent = iter;
			if(iter->key <= key){
				//iter = iter->right;	
				if(iter->right == NULL){
					iter->right = initNode(iter,NULL,NULL,key);	
					break;
				}else{
					iter = iter->right;
				}
			}else if(iter->key > key){
				//iter = iter->left;
				if(iter->left == NULL){
					iter->left = initNode(iter,NULL,NULL,key);	
					break;
				} else{
					iter = iter->left;
				}
			}
		}
	}
}

Node *initNode(Node *parent,Node *left, Node *right, int key){
	Node *theNode = (Node *)malloc(sizeof(Node));
	theNode->parent = parent;
	theNode->left = left;
	theNode->right = right;
	theNode->key = key;

	return theNode;
}

void inOrderWalk(Node *root){
	if(root != NULL){
		inOrderWalk(root->left);
		printf("| %02d ",root->key);
		inOrderWalk(root->right);
	}
}


void printTree(Node *root){
	if(root != NULL){
		printf("\t\t\t\t:%02d ",root->key);
		printf("\n\t\t\t/\t\\\n");
		if(root->left != NULL){
			printf("\t\t %02d ",root->left->key);
		}else{
			printf("\t No left Child");
		}
		if(root->right != NULL){
			printf("\t\t %02d ",root->right->key);
		}else{
			printf("\t\t\t\t\tNo right Child");
		}
		printf("\n");
		printTree(root->left);
		printTree(root->right);

	}
}

Node *findMin(Node *root){
	if(root != NULL){

		if(root->left){
			return findMin(root->left);
		}else{
			return root;
		}
	}else{
		return NULL;
	}
}



void deleteNode(Node *root, int key){

	if(root != NULL){
		printf("Looking at node with key %02d\n, and looking for %02d\n",root->key,key);
		if(key > root->key){
			deleteNode(root->right,key);
		}else if(key < root->key){
			deleteNode(root->left,key);
		}else{
			Node *temp;
			// Here we take care of the base cases
			// One child
			if(root->left != NULL && root->right != NULL){
				printf("This node has two children, %02d and %02d \n",root->left->key,root->right->key);
				temp = findMin(root->right);
				root->key = temp->key;
				deleteNode(root->right,temp->key);
			}else{
				// There is only one or no children
				if(root->left == NULL && root->right == NULL){
					printf("This node has no children\n");
					//make this node's parents point to null
					if(root->parent != NULL){
						if(root->key > root->parent->key){
							root->parent->right = NULL;	
						}else{
							root->parent->left = NULL;
						}
					} 
					free(root);
				}else if(root->left == NULL){
					printf("This node has a right child\n");
					// Everything about the right child should be copied to the current node
					// And the right child should be deleted
					temp = root;
					*root = *(root->right);
					root->parent = temp->parent;
					deleteNode(root->right,root->key);
				}else{
					// Everything about the left child should be copied to the current node
					// And the left child should be deleted
					printf("This node has a left child\n");

					temp = root;
					*root = *(root->left);
					root->parent = temp->parent;
					//root->left = root->left->left;
					//root->right = root->left->right;
					//root->key = root->left->key;
					deleteNode(root->left,root->key);
				}

		
			}

		}
	}
}


Node *findNode(Node *root,int key){
	if(root == NULL){
		return NULL;
	}else{
		if(root->key == key){
			return root;
		}else{
			if(key > root->key){
				return findNode(root->right,key);
			}else{
				 return findNode(root->left,key);
			}
		}
	}
}

Node *findMax(Node *root){
	if(root == NULL){
		return NULL;
	}else{
		if(root->right){
			return findMax(root->right);	
		}else{
			return root;
		} 
	}
}



void deallocateTree(Node *root){
	if(root != NULL){
		deallocateTree(root->left);
		deallocateTree(root->right);
		free(root);
	}
}

int main (){
	
	// Start a tree by making a base node with value 20
	Node *tree = initNode(NULL,NULL,NULL,20);
	insertValue(tree,30);
	insertValue(tree,10);
	insertValue(tree,25);
	insertValue(tree,15);
	insertValue(tree,9);
	insertValue(tree,39);
	insertValue(tree,33);
	insertValue(tree,40);
	insertValue(tree,31);
	insertValue(tree,35);
	insertValue(tree,41);

	printf("The tree contains  ");
	inOrderWalk(tree);
	printf("\n");
	Node *min = findMin(tree);
	if(min) printf("The minimum is %02d\n",min->key);
	Node *max = findMax(tree);
	if(max) printf("The max is %02d\n",max->key);
	Node *temp = findNode(tree,33);
	if(temp) printf("The node with key 33 was found and it has as parent %02d, as right %02d, and as left %02d\n",(temp->parent ? temp->parent->key : -1),(temp->right ? temp->right->key : -1),(temp->left ? temp->left->key : -1));
	deleteNode(tree,30);
	inOrderWalk(tree);
	printf("\n");
	//printTree(tree);
	deallocateTree(tree);

	return 0;
}

