/*
* Program to create and inventory hashfile by applying the transactions listed in
* a data file.
* Author: Simon Haile
* Date: May 30, 2017
*/
#include "hashfile.h"
#include "product.h"
#include "price.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE_LEN 1024
int main(int argc, char *argv[]){

if(argc < 3){
printf("Usage: please type the inventory file name followed by the desired file name fo the hash file.\n");
exit(0);
}
size_t len = MAX_LINE_LEN;
char *inventory = argv[1];
char *hashfile_name = argv[2];
char *line = NULL;
FILE *fp = NULL;
	if(access(inventory, F_OK) != -1){
		fp = fopen(inventory,"r+");
	}
	else{
		printf("Cound not find file named: %s\n",inventory);
		exit(0);
	}
	//create a new hash file using hashfile_name
	FILE *hash_fp = create_hashfile(hashfile_name);
	product_t new_prod;
	//read all lines from the inverntory_file
	while(	getline(&line, &len,fp) != -1){
		line[strlen(line)-1] = '\0';
		read_product(line, &new_prod);
		add_item(hash_fp,new_prod);
	}


	display_hashfile(hash_fp);
}
