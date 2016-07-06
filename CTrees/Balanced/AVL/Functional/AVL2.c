#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX(x,y) ((x) > (y) ? (x) : (y))


typedef struct Node{
	struct Node *parent;
	struct Node *left;
	struct Node *right;
	int height;
	int key;
}Node;

// All from regular binary trees (augmented when needed)
Node *initNode(Node *parent,Node *left, Node *right, int key);
Node *insertValue(Node *tree, int key);
void inOrderWalk(Node *root);
void printTree(Node *root);
void deallocateTree(Node *root);
Node *deleteNode(Node *root, int key);

Node *findMin(Node *root);
Node *findNode(Node *root,int key);
Node *findMax(Node *root);


// New for AVL
int findHeight(Node *root);
int updateHeight(Node *root);
Node *rotateLeft(Node *root);
Node *rotateRight(Node *root);
Node *rebalance(Node *root);
int invalanced(Node *left, Node *right);

// Convenience methods for this implementation
Node *getLeftChild(Node *root);
Node *getRightChild(Node *root);
void adoptChild(Node *parent, Node *child);

Node *getLeftChild(Node *root){
	if(root == NULL) return NULL;
	else{
		return root->left;
	}
}
Node *getRightChild(Node *root){
	if(root == NULL) return NULL;
	else{
		return root->right;
	}
}

Node *rebalance(Node *root){
	if(root != NULL) printf("Rebalancing from %02d\n",root->key);
	Node *baseOfTree = NULL;
	while(root){
		
		updateHeight(root);
		
		//printf("updated height is: %02d\n",updateHeight(root));
		//printf("rebalancing %02d\n",root->key);
		//printf(" with height %02d\n",root->height);
		// If the left child is heavy and breaks the AVL reqs. 
		if(findHeight(root->left) >= 2 + findHeight(root->right)){
			printf("node %02d's Left child is too heavy\n",root->key);
			// eq to height(root->left->left) and height(root->left->left)
				if(root->left){
					printf("This node has a left %02d\n",root->left->key);
				}   
			    if(root->right){
					printf("This node has a right %02d \n",root->right->key);
				}   

				if(findHeight(getLeftChild(root->left)) >= findHeight(getRightChild(root->left)) ){
					printf("Case 1\n");
					rotateRight(root);
				}else{
					printf("Case 2\n");
					rotateLeft(root->left);
					rotateRight(root);
				}				
			
		}else if(findHeight(root->right) >= 2 + findHeight(root->left)){
		//	printf("Right child is too heavy\n");
			printf("node %02d's right child is too heavy\n",root->key);
			if(findHeight(getRightChild(root->right)) >= findHeight(getLeftChild(root->right)) ){
				rotateLeft(root);
			}else{
				rotateRight(root->right);
				rotateLeft(root);
			}
		}
		baseOfTree = root;
		root = root->parent;
	}
	return baseOfTree;
}
void adoptChild(Node *parent, Node *child){
	if(parent != NULL && child != NULL){
		printf("%02d wants to adopt %02d\n",parent->key,child->key);
		if(child->key < parent->key){
			parent->left = child;
			child->parent = parent;
		}else{
			parent->right = child;
			child->parent = parent;
		}

	}
}


// returns the value of the node at the top, since it's no longer the node that we passed
Node *rotateLeft(Node *root){
	if(root != NULL){
		//give right child your parent
		//take his left child if any and make it your right child
		printf("LRotating around %02d\n",root->key);
		Node *r = root->right;
		if(r != NULL){
			printf("Right child had key %02d\n",r->key);
			if(root->parent != NULL){
				adoptChild(root->parent,r);	
			}else{
				r->parent = NULL;
			}
			
			root->parent = r;
			root->right = r->left;
			r->left = root;
			updateHeight(root);
		}
		return r;
	}else return NULL;
}


Node *rotateRight(Node *root){
	if(root){
		// give left child your parent
		// become child's right child
		// take his right child if any and make it your left child
		//printf("RRotating around %02d\n",root->key);
		Node *l = root->left;
		if(l != NULL){
			//printf("Left child had key %02d\n",l->key);
			if(root->parent){
				adoptChild(root->parent,l);				
			}else{
				l->parent = NULL;
			}
			
			//l->parent = root->parent;
			// Now parent needs to point to this child
			root->parent = l;
			root->left = l->right;
			l->right = root;
			printf("Is the problem here?\n");
			updateHeight(root);
			printf("Or here?");
		}
		return l;
	}else return NULL;
}

int findHeight(Node *root){

	if(root){
		return MAX(findHeight(root->left),findHeight(root->right)) + 1;
	}else return -1;

}

int updateHeight(Node *root){
	if(root){
		root->height = findHeight(root);
		printf("Node with key %02d now has height %02d\n",root->key,root->height);
		return root->height;
	}else return -1;
}


Node *insertValue(Node *tree, int key){
	if(tree == NULL){
		tree = initNode(NULL,NULL,NULL,key);
		updateHeight(tree);
		return tree;
	}else{
		Node *iter = tree;
		Node *theNewOne;
		//Node *parent = NULL;
		while(iter != NULL){
			//parent = iter;
			if(iter->key <= key){
				//iter = iter->right;	
				if(iter->right == NULL){
					iter->right = initNode(iter,NULL,NULL,key);	
					theNewOne = iter->right;
					break;
				}else{
					iter = iter->right;
				}
			}else if(iter->key > key){
				//iter = iter->left;
				if(iter->left == NULL){
					iter->left = initNode(iter,NULL,NULL,key);	
					theNewOne = iter->left;
					break;
				}else{
					iter = iter->left;
				}
			}
		}
		//updateHeight(theNewOne);
		return rebalance(theNewOne);
	}
}

Node *initNode(Node *parent,Node *left, Node *right, int key){
	Node *theNode = (Node *)malloc(sizeof(Node));
	theNode->parent = parent;
	theNode->left = left;
	theNode->right = right;
	theNode->key = key;
	theNode->height = 0;
	return theNode;
}

void inOrderWalk(Node *root){
	if(root != NULL){
		inOrderWalk(root->left);
		printf("| %02d - height: %02d\n",root->key,root->height);
		inOrderWalk(root->right);
	}
}


void printTree(Node *root){
	if(root != NULL){
		printf("  %02d\n",root->key);
		printf(" /  \\\n");
		if(root->left != NULL){
			printf("%02d",root->left->key);
		}else{
			printf("NULL");
		}
		if(root->right != NULL){
			printf("   %02d",root->right->key);
		}else{
			printf("   NULL");
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



Node *deleteNode(Node *root, int key){
	Node *theRootTree = root;

	if(root != NULL){
		//printf("Looking at node with key %02d\n, and looking for %02d\n",root->key,key);
		if(key > root->key){
			return deleteNode(root->right,key);
		}else if(key < root->key){
			return deleteNode(root->left,key);
		}else{
			Node *temp;
			// Here we take care of the base cases
			// One child
			if(root->left != NULL && root->right != NULL){
				//printf("This node has two children, %02d and %02d \n",root->left->key,root->right->key);
				temp = findMin(root->right);
				root->key = temp->key;
				return deleteNode(root->right,temp->key);
			}else{
				// There is only one or no children
				if(root->left == NULL && root->right == NULL){
					//printf("This node has no children\n");
					//make this node's parents point to null
					temp = root->parent;
					if(root->parent != NULL){
						if(root->key > root->parent->key){
							root->parent->right = NULL;	
						}else{
							root->parent->left = NULL;
						}
					} 
					free(root);
					return rebalance(temp);

				}else if(root->left == NULL){
					//printf("This node has a right child\n");
					// Everything about the right child should be copied to the current node
					// And the right child should be deleted
					temp = root;
					*root = *(root->right);
					root->parent = temp->parent;
					return deleteNode(root->right,root->key);
				}else{
					// Everything about the left child should be copied to the current node
					// And the left child should be deleted
					//printf("This node has a left child\n");

					temp = root;
					*root = *(root->left);
					root->parent = temp->parent;
					//root->left = root->left->left;
					//root->right = root->left->right;
					//root->key = root->left->key;
					return deleteNode(root->left,root->key);
				}
			}
		}
	}else return NULL;
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
	Node *tree = initNode(NULL,NULL,NULL,41);
	//printf("Height is: %02d\n",tree->height);
	tree = insertValue(tree,65);
	//printf("Height is: %02d\n",tree->height);
	tree = insertValue(tree,20);
	//printf("Height is: %02d\n",tree->height);
	tree = insertValue(tree,50);
	//printf("Height is: %02d\n",tree->height);
	tree = insertValue(tree,29);
	tree = insertValue(tree,11);
	tree = insertValue(tree,26);
	printf("Printing the tree:~~~~~~~~~~~~~~~~~~\n");
	printTree(tree);
	tree = insertValue(tree,23);
	printf("Printing the tree:~~~~~~~~~~~~~~~~~~\n");
	//inOrderWalk(tree);
	printTree(tree);
	tree = insertValue(tree,55);
	printf("Printing the tree:~~~~~~~~~~~~~~~~~~\n");
	printTree(tree);
	tree = insertValue(tree,9);
	tree = insertValue(tree,8);
	printf("Printing the tree:~~~~~~~~~~~~~~~~~~\n");
	printTree(tree);
	printf("Deleting 2 nodes \n");
	tree = deleteNode(tree,50);
	tree = deleteNode(tree,65);
	printf("Done deleting\n");
	printTree(tree);
	inOrderWalk(tree);
	
	/*
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
	printf("Deleted 30\n");
	temp = findNode(tree,33);
	if(temp) printf("The node with key 33 was found and it has as parent %02d, as right %02d, and as left %02d\n, and height %02d\n",(temp->parent ? temp->parent->key : -1),(temp->right ? temp->right->key : -1),(temp->left ? temp->left->key : -1), temp->height);
	printf("The height of node 31 is: %02d\n",findHeight(findNode(tree,31)));
	printf("The height of node 33 is: %02d\n",findHeight(findNode(tree,33)));
	printf("The height of node 15 is: %02d\n",findHeight(findNode(tree,15)));
	deleteNode(tree,35);
	temp = findNode(tree,33);
	if(temp) printf("The node with key 33 was found and it has as parent %02d, as right %02d, and as left %02d\n, and height %02d\n",(temp->parent ? temp->parent->key : -1),(temp->right ? temp->right->key : -1),(temp->left ? temp->left->key : -1), temp->height);

	//printTree(tree);
	*/
	deallocateTree(tree);

	return 0;
}

