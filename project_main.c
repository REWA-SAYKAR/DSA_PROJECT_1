#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<fcntl.h>//For LINUX file handling
#include<unistd.h>//for LINUX file handling(w)
#include "ADT.h"

int main(int argc, char *argv[]){

	char *to_be_comp;
	to_be_comp = argv[1];
	// Opening to_be_compressed file in read-only mode
        //char to_be_comp[] = "sample3.txt";
        int fd1 = open(to_be_comp, O_RDONLY);
        if (fd1 == -1) {
                perror("Failed to open to_be_compressed file(in read mode)\n");
                exit(1);
	}

	//creating hashmap from to_be_compressed file
        // Inserting each character from file into the hashmap
        char c;
        while(read(fd1,&c,sizeof(char)) != 0){
                insert(c);
        }
        close(fd1);

	//creating the pq from the hashmap
	PQ *t;
        init(&t);
        t = create_pq_from_hashmap(t);

	//bulding the huffamn tree from the pq
        node*nn=buildTree(t);

	//now the main compression part
        int arr_t[256];
        block *b;
        init_block(&b);

        char temp_write[] = "temp.txt";
        int temp_w = open(temp_write,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (temp_w == -1) {
                perror("Failed to open temporary file(in write mode)\n");
                exit(1);
        }

        printCodesIntoFile(temp_w,nn,arr_t,0,b);
        close(temp_w);

	//MAX referes to the length of the longest huffman code amoung all the huffman codes generated
	//we extract this information from the block data structure
	//in which all the codes are saved in the form of an int array
        int MAX;
        MAX = get_MAX(b);

	//we need to open the to_be_compressed file again in read mode
	//......as we have read it all once while creating the hashmap
        char to_be_comp_[] = "sample3.txt";
        int fd0 = open(to_be_comp_, O_RDONLY);
        if (fd0 == -1) {
                perror("Failed to open to_be_compressed file(in read mode)\n");
                exit(1);
        }

	//opening compressed file in write mode
        char sample_compressed[] = "sample_compressed.txt";
        int fd2 = open(sample_compressed,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd2 == -1) {
                perror("Failed to open compressed file(in write mpde)\n");
                exit(1);
        }

        compressFile(fd0,fd2,b);
        close(fd0);
        close(fd2);

	//-----------------------now for the decmopression----------------------------------------
        int choice;
        printf("\nDo you wish to continue decompression [1/0]? --> ");
        scanf("%d",&choice);
        if(choice == 1){
		//now opening the temp.txt file in read mode
                char temp_read[] = "temp.txt";
                int temp_r = open(temp_read,O_RDONLY);
                if (temp_r == -1) {
                        perror("Failed to open temporary file(in read mode)\n");
                        exit(1);
                }

                int size;
                size = b->k;//i.e. total number of huffman codes built
                Tree *tree = ReBuildHuffmanTree(temp_r, size, MAX);
                close(temp_r);

		//f is the total number of characters that are to be in the decompressed file
                int f;
                f = get_f();

                char sample_compressed_[] = "sample_compressed.txt";
                int fd3 = open(sample_compressed_, O_RDONLY);
                if (fd3 == -1) {
                        perror("Failed to open compressed file(in read mode)\n");
                        exit(1);
                }

                char sample_decompressed[] = "sample_decompressed.txt";
                int fd4 = open(sample_decompressed,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                if (fd4 == -1) {
                        perror("Failed to open decompressed file(in write mode)\n");
                        exit(1);
                }

                decompressFile(fd3,fd4,f,tree);

		close(fd3);
                close(fd4);
        }
        return 0;
}
