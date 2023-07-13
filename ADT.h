#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <fcntl.h>//for dat weird file handling
#include<math.h>
#include<unistd.h>//same file hand.

#define TABLE_SIZE 1000000

//implementation by linked list
//array of linked list to store key-value pair....key is our character and value is its frequency in the character string given
typedef struct node1 {
    char key;
    int freq;
    struct node1 *next;
} node_t;

typedef struct node{
        int data;
        char word;
        struct node *left;
        struct node *right;
}node;

typedef struct PQ{
        node** arr;
        int size;
}PQ;

typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;

// Structure to store codes in compressed file
typedef struct code {
    char k;
    int l;
    int code_arr[16];
    struct code* p;
} code;

typedef struct block{
        code* front;
        code* rear;
        int k;//len of block[] data strc
        //code block_arr[256];
}block;


void insert(char key);
void init(PQ **t);
PQ *create_pq_from_hashmap(PQ *t);
node *buildTree(PQ *t);
void init_block(block **b);
void printCodesIntoFile(int temp_w, node *root, int t[], int top, block *b);
int get_MAX(block *b);
Tree *ReBuildHuffmanTree(int fd3, int size, int MAX);
int get_f();

void compressFile(int fd1, int fd2,block *b);
void decompressFile(int fd3, int fd4, int f, Tree *tree);
