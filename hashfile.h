/*
 * hashfile.h
 * 
 * header file for inventory hash table file
 * 
 * CSCI 352, Spring 2017, Assignment 3
 * 
 * David Bover, May 2017
 * 
 */

#ifndef HASHFILE_DEF
#define HASHFILE_DEF	1

#include "product.h"

// creates and initilizes a new hashfile for product_t data
// opens for read/write
FILE* create_hashfile(char *name);

// opens an existing hashfile of product_t data for read/write
FILE* open_hashfile(char *name);

// add a new product_t data item to the file
// return 0 on error, 1 if OK
int add_item(FILE *fp, product_t product);

// display the entire contents of the file in text
// one line per product_t data item
void display_hashfile(FILE *fp);

// display the offsets in the free list of blocks
void display_free_list(FILE *fp);

// search for the key in the hashed file
// return location of product_t data in file, 0 if not found
long find_item(FILE *fp, char *key);

// delete the product_t data item matching the key from the hashed file
// return 1 on success, 0 on failure
int delete_item(FILE *fp, char *key);

// update the sales figures for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_sales(FILE *fp, char *key, int quantity);

// update the stock for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_stock(FILE *fp, char *key, int quantity);

// update the price for the product_t data item matching the key
// return 1 on success, 0 on failure
int update_price(FILE *fp, char *key, price_t newprice);

// check the sales for each product in the file
// if sales not updated, assume 0 sales for the month
void check_sales(FILE *fp);

#endif
