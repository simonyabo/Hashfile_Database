/*
* Program to update an existing inventory hashfile by applying the transactions listed in
* a data file.
* Author: Simon Haile
* Date: May 30, 2017
*/
#include "hashfile.h"
#include "product.h"
#include "price.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE_LEN 1024
#define PROD_QUANTITY 2
int run_updates(FILE *trans_fp, FILE *hash_fp);

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Usage: please type the transaction file name followed by the hash file name.\n");
		exit(0);
	}
	char *transactions_file = argv[1];
	char *hashfile_name = argv[2];
	FILE *trans_fp;
	FILE *hash_fp;
		if(access(transactions_file, F_OK) != -1){
			trans_fp = fopen(transactions_file,"r+");
		}
		else{
			printf("Cound not find file named: %s\n",transactions_file);
			exit(0);
		}
		if(access(hashfile_name, F_OK) != -1){
		hash_fp = open_hashfile(hashfile_name);
		}
		else{
			printf("Cound not find file named: %s\n",hashfile_name);
			exit(0);
		}
		run_updates(trans_fp,hash_fp);
		check_sales(hash_fp);
		display_free_list(hash_fp);
		display_hashfile(hash_fp);
}
//goes through all the lines in a transactions file
//and updates the necessary location in the file
int run_updates(FILE *trans_fp, FILE *hash_fp){
	size_t len = MAX_LINE_LEN;
	char *line = NULL;
	char *input_line = NULL;
	char **tokens = NULL;
	char *transaction = NULL;
	int dollars, cents;
	product_t new_prod;

		while(	getline(&line, &len,trans_fp) != -1){
				input_line = line;
				tokens = tokenize(line," ");
				transaction = tokens[0];
				if (strcmp(transaction,"SALE")==0){
					(tokens[1])[8] = '\0';
					update_sales(hash_fp,tokens[1],atoi(tokens[PROD_QUANTITY]));
				}
				else if (strcmp(transaction,"DELIVERY")==0){
					(tokens[1])[8] = '\0';
					update_stock(hash_fp,tokens[1],atoi(tokens[PROD_QUANTITY]));
				}
				else if (strcmp(transaction,"PRICE")==0){
					// get the price dollars and cents
					if (sscanf(tokens[2], "%d.%d", &dollars, &cents) < 2) return 0;
					update_price(hash_fp,tokens[1],new_price(dollars,cents,0) );
				}
				else if (strcmp(transaction,"DELETE")==0){
					(tokens[1])[8] = '\0';
					delete_item(hash_fp,tokens[1]);
				}
				else if (strcmp(transaction,"NEWPRODUCT")==0){
					input_line += (strlen("NEWPRODUCT")+1);
					read_new_product(input_line, &new_prod);
					add_item(hash_fp, new_prod);
				}
			}
		return 1;
}
