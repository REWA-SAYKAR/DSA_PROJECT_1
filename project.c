#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<fcntl.h>//For LINUX file handling
#include<unistd.h>//for LINUX file handling(w)
#include "ADT.h"

//===========================HASHMAP FUNCTIONS=============================================================================================
//implementation by linked list
//array of linked list to store key-value pair....key is our character and value is its frequency in the character string given

node_t *table[TABLE_SIZE];

//function to take character key and returns an index in an array
int hash(char key) {
    return (int)key % TABLE_SIZE;
}

//creates a newnode of hashmap and return that node which has been created
node_t *create_node(char key, int freq) {
    node_t *new_node = malloc(sizeof(node_t));
    new_node->key = key;
    new_node->freq = freq;
    new_node->next = NULL;
    return new_node;
}

//to insert a newnode in the table
void insert(char key) {
    int index = hash(key);
    node_t *head = table[index];

    // If the key is already in the table, increase its frequency
    for (node_t *node = head; node != NULL; node = node->next) {
        if (node->key == key) {
            node->freq++;
            return;
        }
    }

    // If the key is not in the table...we add it
    node_t *new_node = create_node(key, 1);//as its not in table the initial freq is by default ONE
    new_node->next = head;
    table[index] = new_node;
}

//to traverse the table
void print_table() {
        for (int i = 0; i < TABLE_SIZE; i++) {
        node_t *node = table[i];
        while (node != NULL) {
            printf("Key --> %c, Frequency --> %d\n", node->key, node->freq);
            node = node->next;
        }
    }
}

//to get he total frequency
//we'll need this for the decompression part
int  get_f() {
        int freq_f = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
                node_t *node = table[i];
                while (node != NULL) {
                        freq_f = freq_f + node->freq;
                        node = node->next;
                }
        }
        printf("\nf-->%d\n",freq_f);
        return freq_f;
}

//========================PRIORITY QUEUE FUNCTIONS=========================================================================================
//initializing the priority queue
void init(PQ** t){
        *t=(PQ*)malloc(sizeof(PQ));
        (*t)->arr=(node**)malloc(sizeof(node*)*1000);
        (*t)->size=0;
        return;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//complementary functions
int isEmpty(PQ t){
        return t.size=0;
}

int size(PQ t){
         return t.size;
}

node* getMin(PQ* t){
         if(isEmpty(*t)) return NULL;
         return t->arr[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------


void insert1(PQ *t,node* p){
       t->arr[t->size]=p;
       t->size=(t->size)+1;
       int childIndex=t->size-1;
       int parentIndex=(childIndex-1)/2;
       while(parentIndex>=0 && (t->arr[parentIndex]->data)>(t->arr[childIndex]->data)){

                node *p=t->arr[parentIndex];
                node *q=t->arr[childIndex];
                t->arr[parentIndex]=q;
                t->arr[childIndex]=p;

                childIndex=parentIndex;
                parentIndex=(childIndex-1)/2;
                }
                return ;
}


void removeMin(PQ *t){
     node *p=t->arr[t->size-1];
     t->arr[t->size-1]=NULL;
     t->arr[0]=p;

    // printf("%d",t->arr[0]->data);
     t->size=t->size-1;
     int parent=0;
     int leftchild=1;
     int rightchild=2;
     int min=parent;
     while(leftchild<t->size){
            if(t->arr[leftchild]->data<t->arr[min]->data){
                 min=leftchild;
	    }
            if(rightchild<t->size && t->arr[rightchild]->data<t->arr[leftchild]->data){
                 min=rightchild;
	    }
            if(min==parent) break;
            else{
                 node* m=t->arr[parent];
                 node* n=t->arr[min];
                 t->arr[parent]=n;
                 t->arr[min]=m;

                 parent=min;
                 leftchild=2*parent+1;
                 rightchild=2*parent+2;
	    }
     }
     return;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//building the Huffman tree using huffman algorithm
node* buildTree(PQ *t){
    while(t->size>1){
         node *nn=(node*)malloc(sizeof(node));
         nn->left=NULL;
         nn->right=NULL;

         node* p=getMin(t);
         removeMin(t);

         node* q=getMin(t);
         removeMin(t);

         nn->data=p->data+q->data;
         nn->word='\0';
         nn->left=p;
         nn->right=q;

         insert1(t,nn);
    }
    return getMin(t);
}

//-----------------------------------------------------------------------------------------------------------------------------------------

PQ *create_pq_from_hashmap(PQ *t){
	for (int i = 0; i < 1000000; i++) {
		node_t *box = table[i];
        	while (box != NULL) {
			node* p=(node*)malloc(sizeof(node));
            		p->data=box->freq;
            		p->word=box->key;
            		insert1(t,p);
            		box = box->next;
        	}
        }
        return t;
}

//complementary functions
int isLeaf(node* root){
        while(root){

                if(root->left == NULL && root->right == NULL){
                        return 1;
                }
                return 0;
        }
}

int convertBinaryToDecimal(int code_arr[], int top){
        int sum = 0;
        for(int i=0;i<top;i++){
                sum +=((code_arr[i])*pow(2,(top-i-1)));
        }

        return sum;
}

int max(int a,int b){
        if(a>=b){
                return a;
        }
        return b;
}

//===========================BLOCk function========================================================================================================================================
void init_block(block **b){
        (*b) = (block*)malloc(sizeof(block));

        (*b)->front = NULL;
        (*b)->rear = NULL;
        (*b)->k = 0;
        //(*b)->block_arr = (int*)malloc(sizeof(int)*256);
        return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//now Recursive Function to Print Binary Codes into Compressed File
void printCodesIntoFile(int temp_w, node* root,int t[],int top,block *b){
        int dec;
        int i;
    	if (root->left) {
        	t[top] = 0;
        	printCodesIntoFile(temp_w, root->left, t, top + 1,b);
    	}

    	if (root->right) {
        	t[top] = 1;
        	printCodesIntoFile(temp_w, root->right, t, top + 1,b);
    	}

    	if (isLeaf(root)) {
        	code *data = (code*)malloc(sizeof(code));
        	data->p = NULL;
        	data->k = root->word;
        	write(temp_w, &data->k, sizeof(char));
        	for (i = 0; i < top; i++) {
            		data->code_arr[i] = t[i];
        	}
        	write(temp_w, &top, sizeof(int));
        	dec = convertBinaryToDecimal(data->code_arr, top);
        	write(temp_w, &dec, sizeof(int));
        	data->l = top;
        	data->p = NULL;
        	if (b->k == 0) {
            		b->front = b->rear = data;
            		(b->k)++;
        	}
        	else {
            		b->rear->p = data;
            		b->rear = b->rear->p;
            		(b->k)++;
        	}
	}
}

//now the main compression function
//in this function, concept of bit shifting is used
void compressFile(int fd1, int fd2,block *b){
        unsigned char a;
        char n;
    	int h = 0, i;

    	// Codes are written into file in bit by bit format
    	while (read(fd1, &n, sizeof(char)) != 0) {
		b->rear = b->front;
        	while (b->rear->k != n && b->rear->p != NULL) {
			b->rear = b->rear->p;
        	}
        	if (b->rear->k == n) {
                //printf("\n-----charac found----\n");
            		for (i = 0; i < b->rear->l; i++) {
				if (h < 7) {
					if (b->rear->code_arr[i] == 1) {
                        			a++;
                        			a = a << 1;
                        			h++;
                    			}
                    			else if (b->rear->code_arr[i] == 0) {
                        			a = a << 1;
                        			h++;
                    			}
                		}
                		else if (h == 7) {
                    			if (b->rear->code_arr[i] == 1){
                        			a++;
                        			h = 0;
                    			}
                    			else {
                        			h = 0;
                    			}
                    			write(fd2, &a, sizeof(char));
                    			a = 0;
                		}
			}
		}
	}
    	for (i = 0; i < 7 - h; i++) {
        	a = a << 1;
	}

	write(fd2, &a, sizeof(char));
    	printf("\nFile compressed successfully\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------

//===========================now for the decompression====================================================================================
//complementary functions
int get_MAX(block *b){
        int MAX = 0;
        int l;
        code *c=b->front;
        for(int i=0;i<b->k;i++){
                l=c->l;
                if(MAX < l){
                        MAX = l;
                }
                c=c->p;
        }
        printf("\n MAX --> %d \n",MAX);

        return MAX;
}

void convertDecimalToBinary(int bin[], int dec, int len){
	int i = len-1;
    	while(dec > 0 && i>=0) {
        // storing remainder in binary array
        	bin[i] = dec % 2;
        	dec = dec / 2;
        	i--;
    	}
}
int isRoot(Tree *tree_temp,Tree *tree){
        if(tree_temp==tree){
                return 1;
        }
        else{
                return 0;
        }
}

//----------------------------------------------------------------------------------------------------------------------------------------
//first we'll re build the huffman tree from the temp.txt file
void ExtractCodesFromFile(int temp_r,Tree *t){
        if(t){
                read(temp_r, &t->g, sizeof(char));
                read(temp_r, &t->len, sizeof(int));
                read(temp_r, &t->dec, sizeof(int));
        }
        else{
                t = (Tree*)malloc(sizeof(Tree));
                t->f = NULL;
                t->r = NULL;
                read(temp_r, &t->g, sizeof(char));
                read(temp_r, &t->len, sizeof(int));
                read(temp_r, &t->dec, sizeof(int));
        }
}

// Function to rebuild the Huffman tree
Tree *ReBuildHuffmanTree(int fd3, int size, int MAX){
	int i = 0, j, k;
    	Tree *tree = (Tree*)malloc(sizeof(Tree));
    	Tree *tree_temp = NULL;
    	tree_temp = tree;
    	tree->f = NULL;
    	tree->r = NULL;
    	Tree *t = (Tree*)malloc(sizeof(Tree));
    	t->f = NULL;
    	t->r = NULL;
    	for (k = 0; k < size; k++) {
		//again at the root node
		tree_temp = tree;

            	ExtractCodesFromFile(fd3,t);

		//initializing both buffer arrays with MAX length
		//so there wont be any need of realloc
		//and storing zero in each of their indices
		//(we can also do calloc here instead)
		int bin[MAX], bin_con[MAX];
        	for (i = 0; i < MAX; i++) {
            		bin[i] = bin_con[i] = 0;
        	}

		//now getting the huffman code in bin[] first
		convertDecimalToBinary(bin, t->dec, t->len);

		//putting tha binary code into bin_con[]
		for (i = 0; i < t->len; i++) {
            		bin_con[i] = bin[i];
        	}

		//now rebulding the huffman tree from the code
        	for (j = 0; j < t->len; j++) {
            		if (bin_con[j] == 0) {
				if (tree_temp->f == NULL) {
                    			tree_temp->f = (Tree*)malloc(sizeof(Tree));
                		}
                		tree_temp = tree_temp->f;
            		}
            		else if (bin_con[j] == 1) {
                		if (tree_temp->r == NULL) {
                    			tree_temp->r = (Tree*)malloc(sizeof(Tree));
                		}
                		tree_temp = tree_temp->r;
            		}
        	}
        	tree_temp->g = t->g;
        	tree_temp->len = t->len;
        	tree_temp->dec = t->dec;
        	tree_temp->f = NULL;
        	tree_temp->r = NULL;
        	tree_temp = tree;
	}
	return tree;
}

//Function to Decompress the Compressed File:
void decompressFile(int fd3, int fd4, int f, Tree *tree){
        //first checking if the tree is actually built
	if(!tree){
                return;
        }

        int inp[8], i, k = 0;
    	unsigned char p;
    	read(fd3, &p, sizeof(char));
    	convertDecimalToBinary(inp, p, 8);
    	Tree *tree_temp = tree;
    	for (i = 0; i < 8 && k < f; i++) {
		if(!isRoot(tree_temp,tree)){
                	if (i != 7) {
                        	if (inp[i] == 0) {
                                	tree_temp = tree_temp->f;
                        	}
                        	if (inp[i] == 1) {
                                	tree_temp = tree_temp->r;
                        	}
                	}
                	else {
                        	if (inp[i] == 0) {
                                	tree_temp = tree_temp->f;
                        	}
                        	if (inp[i] == 1) {
                                	tree_temp = tree_temp->r;
                        	}
                        	if(read(fd3, &p, sizeof(char)) != 0) {
                                	convertDecimalToBinary(inp, p, 8);
                                	i = -1;
                        	}
                        	else {
                                	break;
                        	}
                	}
            	}
        	else {
			k++;
            		write(fd4, &tree_temp->g, sizeof(char));
            		tree_temp = tree;
            		i--;
        	}
	}
	printf("\nFile decompressed successfully\n");
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================================

