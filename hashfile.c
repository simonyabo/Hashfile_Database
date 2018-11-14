
/*
* hashfile.c Implementation of a hashfile to maintain clothing store inventory.
* Authour: Simon Haile
* Date May 30, 2017
*/
#include "hashfile.h"
#include "product.h"
#include "price.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 12
#define HASH_MOD 11
#define MONTHS 12
long hash_code(char* p_code);
void shift_sales(int arr[]);
int print_details(product_t *prod_ptr);
int write_empty_product(FILE *fp, long offset, long next);
int add_to_free_list(FILE *fp, long offset);
long get_free_list(FILE *fp);
long update_free_list(FILE *fp, product_t new);
int init_add(FILE *fp, product_t product);
void display_free_list(FILE *fp);

// creates and initilizes a new hashfile for product_t data
// opens for read/write
FILE* create_hashfile(char *name){
	long hash_table[HASH_TABLE_SIZE];
	for(long i=0; i<HASH_TABLE_SIZE; i++){
		hash_table[i] = 0;
	}
	FILE *fp = fopen(name, "w+");
	fwrite(hash_table, sizeof(long),(sizeof(hash_table)/sizeof(hash_table[0])),fp);
	rewind(fp);
  	return fp;
}
// opens an existing hashfile of product_t data for read/write
FILE* open_hashfile(char *name){
	return fopen(name,"r+");
}
// returns the head of the free list - the offset of the head could be 0
// if there are no freed blocks.
long get_free_list(FILE *fp){
	long f_head = 0;
	fseek(fp,0,SEEK_SET);
	fread(&f_head,sizeof(long),1,fp);
	return f_head;
}
//updates the free list when a block is used up. Writes the new product,
//returns the offset at which it is written if ok. Called only if there is
//atleast on free block.
long update_free_list(FILE *fp, product_t new){
	long write_to_offset = get_free_list(fp);
	product_t empty_prod;
	fseek(fp,write_to_offset,SEEK_SET);
	fread(&empty_prod,sizeof(product_t),1, fp);
	fseek(fp,write_to_offset,SEEK_SET);
	fwrite(&new,sizeof(product_t),1,fp);

	fseek(fp,0,SEEK_SET);
	long t_next = empty_prod.next;
	fwrite(&t_next,sizeof(long),1,fp);
	return write_to_offset;
}
// adds the initial new item to the hash file. uses the free list of blocks
// if there are any. returns 1 if ok, 0 on error.
int init_add(FILE *fp, product_t product){

	char * p_code = product.code;
	long hashi = hash_code(p_code);
	long pos = -1;
	long *pos_ptr = &pos;
	long written_to = 0;
	fseek(fp, (hashi*sizeof(long)),SEEK_SET );
	fread(pos_ptr, sizeof(long),1,fp);
	if(*pos_ptr == 0){
		if(get_free_list(fp) == 0){//there is no free blocks in the free list
			fseek(fp,0,SEEK_END);
			long end_file_ofst = ftell(fp);
			fwrite(&product, sizeof(product_t),1,fp);
			fseek(fp, (hashi*sizeof(long)),SEEK_SET );
			fwrite(&end_file_ofst,sizeof(long),1,fp);
			return 1;
		}
		else{

			written_to = update_free_list(fp,product);
			fseek(fp, (hashi*sizeof(long)),SEEK_SET );
			fwrite(&written_to,sizeof(long),1,fp);
			return 1;
		}
	}
	return 0;
}

// add a new product_t data item to the file
// return 0 on error, 1 if OK
int add_item(FILE *fp, product_t product){
	long fin_off = 0;
	char * p_code = product.code;
	long hashi = hash_code(p_code);
	long pos = -1;
	long *pos_ptr = &pos;
	long written_to = 0;
	fseek(fp, (hashi*sizeof(long)),SEEK_SET );
	fread(pos_ptr, sizeof(long),1,fp);
	if(*pos_ptr == 0){
		init_add(fp,product);
	}
	else{//read the product that is at pos_ptr(in the hash index)
		fseek(fp, pos, SEEK_SET);
		product_t *curr = (product_t *)malloc(sizeof(product_t));
		fread(curr, sizeof(product_t),1,fp);
		if(strcmp(curr->code,product.code) == 0){
			printf("An item with code: %s already exists\n",curr->code);
			return 0;
		}
		else{
				fseek(fp, pos, SEEK_SET);
				fin_off = ftell(fp);
			while(curr->next != 0){
				fseek(fp, (curr->next), SEEK_SET);
				fin_off = ftell(fp);
				fread(curr, sizeof(product_t),1,fp);
				if(strcmp(curr->code,product.code) == 0){
					printf("An item with code: %s already exists\n",curr->code);
					return 0;
				}
			}//update current's next to be the end of the file, and write the new product there
				if(get_free_list(fp) == 0){
					fseek(fp,0,SEEK_END);
					long end_file = ftell(fp);
					fwrite(&product, sizeof(product_t),1,fp);
					curr->next = end_file;
					fseek(fp,fin_off,SEEK_SET);
					fwrite(curr, sizeof(product_t),1,fp);
				}
				else{
					written_to = update_free_list(fp,product);
					curr->next = written_to;
					fseek(fp,fin_off,SEEK_SET);
					fwrite(curr, sizeof(product_t),1,fp);
				}
		}
			return 1;
	}
	return 0;
}
// display the offsets of the free list of blocks in the file.
void display_free_list(FILE *fp){
	printf("*********************DISPLAYING FREE LIST********************\n");
	product_t product;
	long pos = 0;
	fseek(fp,0,SEEK_SET);
	fread(&pos,sizeof(long),1,fp);
		if(pos != 0){
			fseek(fp,pos,SEEK_SET);
			fread(&product, sizeof(product_t),1,fp);
			printf("[%ld]-->",pos);
			while(product.next != 0){
				pos = product.next;
				fseek(fp,pos,SEEK_SET);
				fread(&product, sizeof(product_t),1,fp);
				printf("[%ld]-->",pos);
			}
			printf("\n");
		}

}
// display the entire contents of the file in text
// one line per product_t data item
void display_hashfile(FILE *fp){
		printf("*********************DISPLAYING HASHFILE*********************\n");
		product_t *prod_ptr = (product_t *)malloc(sizeof(product_t));
		long pos =0;
		long *pos_ptr = &pos;
	for(int i=1;i<HASH_TABLE_SIZE;i++){
		fseek(fp,(i*sizeof(long)),SEEK_SET);
		//	printf("fseek sizeof(long): %ld\n", *pos_ptr);
		fread(pos_ptr,sizeof(long),1,fp);
		if(*pos_ptr != 0){
			fseek(fp,*pos_ptr,SEEK_SET);
			fread(prod_ptr, sizeof(product_t),1,fp);
			print_details(prod_ptr);
			while(prod_ptr->next != 0){
				*pos_ptr = prod_ptr->next;
				fseek(fp,*pos_ptr,SEEK_SET);
				fread(prod_ptr, sizeof(product_t),1,fp);
				print_details(prod_ptr);
			}
		}
	}
}
//displays the details of a product struct.
int print_details(product_t *prod_ptr){
	if(prod_ptr == NULL)
		return 0;
	printf("%s ",prod_ptr->code);
	printf("%s ",prod_ptr->desc);
	if (prod_ptr->price.negative)
		printf("-");
		printf("$%d.%d ", prod_ptr->price.dollars, prod_ptr->price.cents);
		printf("%d ", prod_ptr->stock);

	for(int i=0; i<12; i++)
		printf("%d ", (prod_ptr->sales[i]));
		printf("\n");
		return 1;
}
// search for the key in the hashed file
// return location of product_t data in file, 0 if not found
long find_item(FILE *fp, char *key){
	long pos = -1;
	long *pos_ptr = &pos;
	int hashi = hash_code(key);
	product_t *prod_ptr = (product_t *)malloc(sizeof(product_t));

	fseek(fp, (hashi * sizeof(long)),SEEK_SET );
	fread(pos_ptr, sizeof(long), 1, fp);
	if(*pos_ptr == 0)
		return 0;
	else{
		long offset =0;
		fseek(fp,*pos_ptr, SEEK_SET);
		offset = ftell(fp);
		fread(prod_ptr, sizeof(product_t), 1, fp);
		if(strcmp(prod_ptr->code,key) == 0)
			return offset;
		else{
			while(prod_ptr->next != 0){
				fseek(fp, prod_ptr->next, SEEK_SET);
				offset = ftell(fp);
				fread(prod_ptr, sizeof(product_t), 1, fp);
				if(strcmp(prod_ptr->code,key) == 0)
					return offset;

			}
		}
	}
	return 0;
}
// delete the product_t data item matching the key from the hashed file
// return 1 on success, 0 on failure
int delete_item(FILE *fp, char *key){
	//create ttwo pointers, prev points to null and curr points to the cuurent offset
	long prev = 0;
	long curr = 0;
	product_t prod;
	long found_at = find_item(fp,key);

	if(found_at == 0){
		printf("delete_item: Could not locate item with code: %s\n", key);
		return 0;
	}
	fseek(fp,(hash_code(key)*(sizeof(long))),SEEK_SET);
	fread(&curr, sizeof(long),1,fp);

	fseek(fp,curr,SEEK_SET);
	fread(&prod, sizeof(product_t),1,fp);
	while(curr != found_at){
		prev = curr;
		curr = prod.next;
		fseek(fp,curr,SEEK_SET);
		fread(&prod, sizeof(product_t),1,fp);
	}//leaving the loop indicates curr == found_at and prev is the precessor
	long currs_next = prod.next;
	if(prev == 0){
		fseek(fp,(hash_code(key)*(sizeof(long))),SEEK_SET);
		fwrite(&currs_next,sizeof(long),1,fp);
		add_to_free_list(fp,found_at);
		return 1;
	}
	else{
		fseek(fp,prev,SEEK_SET);
		fread(&prod,sizeof(product_t),1,fp);
		prod.next = currs_next;
		fseek(fp,prev,SEEK_SET);
		fwrite(&prod,sizeof(product_t),1,fp);
		add_to_free_list(fp,found_at);

		return 1;
	}
	return 0;
}
// update the sales figures for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_sales(FILE *fp, char *key, int quantity){
	long prod_pos = find_item(fp,key);
	if(prod_pos==0){
		printf("update_sales: Could not locate item with code: %s\n",key);
		return 0;
	}
	else{
		product_t prod;
		fseek(fp,prod_pos,SEEK_SET);
		fread(&prod,sizeof(product_t),1, fp);
		if(prod.sales_updated == 0)
			shift_sales(prod.sales);
		prod.sales[0] += quantity;
		prod.sales_updated = 1;
		prod.stock -= quantity;
		fseek(fp,prod_pos,SEEK_SET);
		fwrite(&prod,sizeof(product_t),1,fp);
	}
	return 1;
}
//shifts the contents of each index in the array monthly sales to the right by one.
//the zeroth index is set to 0 and the last index is deleted.
void shift_sales(int arr[]){
	for(int i=MONTHS-1; i>0; i--)
		arr[i] = arr[i-1];
	arr[0] = 0;
}
//hash function to determine the location of a product in the hash table section
//of the file. The hash table section stores offset values for where products are written
//This function does not hash to the zeroth index- that is resereved for the head of the free block list
long hash_code(char* p_code){
	p_code +=2;
	return 	(atoi(p_code)%HASH_MOD)+1;
}
// update the stock for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_stock(FILE *fp, char *key, int quantity){
	product_t prod;
	long prod_pos = find_item(fp,key);
	if(prod_pos==0){
		printf("update_stock: Could not locate item with code: %s\n",key);
		return 0;
	}

	else{
		fseek(fp, prod_pos,SEEK_SET);
		fread(&prod,sizeof(product_t),1,fp);
		prod.stock += quantity;

		fseek(fp,prod_pos, SEEK_SET);
		fwrite(&prod,sizeof(product_t),1,fp);
		return 1;
	}
return 0;
}
// update the price for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_price(FILE *fp, char *key, price_t newprice){
	product_t prod;
	long prod_pos = find_item(fp,key);
	if(prod_pos){
		fseek(fp, prod_pos,SEEK_SET);
		fread(&prod,sizeof(product_t),1,fp);
		prod.price = newprice;

		fseek(fp,prod_pos, SEEK_SET);
		fwrite(&prod,sizeof(product_t),1,fp);
		return 1;
	}
	printf("update_price: Could not locate item with code: %s\n",key);
	return 0;

}

// check the sales for each product in the file
// if sales not updated, assume 0 sales for the month
void check_sales(FILE *fp){
	long prod_pos = 0;
	product_t prod;
	for(int i=1; i<HASH_TABLE_SIZE; i++){
		fseek(fp, (i*sizeof(long)), SEEK_SET);
		fread(&prod_pos,sizeof(long),1,fp);
		if(prod_pos != 0){
			do{
				fseek(fp,prod_pos,SEEK_SET);
				fread(&prod, sizeof(product_t),1,fp);
				if(prod.sales_updated == 0){
					shift_sales(prod.sales);
					fseek(fp,prod_pos,SEEK_SET);
					fwrite(&prod,sizeof(product_t),1,fp);
				}
			}
			while((prod_pos = prod.next));

		}
	}
}
//write a 'dummy' product to indicate a free location in the file, and the next free location.
//return 1 on success.
int write_empty_product(FILE *fp, long offset, long next){

	price_t n_price = new_price(0,0,0);
	int sale_arr[12]= {1,2,3,4,5,6,7,8,9,10,11,12};
	category_t cat = {6};
	product_t emp_prod = new_product("WL000000","EMPTY",n_price,cat,0,sale_arr);

	emp_prod.next = next;
	fseek(fp,offset,SEEK_SET);
//	printf("HERE in write_empty_product emp.next si: %ld\n",emp_prod.next);
	fwrite(&emp_prod,sizeof(product_t),1,fp);
	return 1;
}
//adds a block of file space to the free list when a product is deleted.
//returns 1 if ok, 0 on error.
int add_to_free_list(FILE *fp, long offset){
	long free_list_head = get_free_list(fp);

	if(free_list_head == 0){
		fseek(fp,0,SEEK_SET);
		fwrite(&offset,sizeof(long),1,fp);
		write_empty_product(fp,offset,0);
		return 1;
	}
	else{
		write_empty_product(fp,offset,free_list_head);
		fseek(fp,0,SEEK_SET);
		fwrite(&offset,sizeof(long),1,fp);
		return 1;
	}
	return 0;
}
