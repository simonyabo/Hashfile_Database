/*
 * product.h
 * 
 * header file for inventory product data item
 * 
 * CSCI 352, Spring 2017, Assignment 3
 * 
 * David Bover, May 2017
 * 
 */

#ifndef PRODUCT_DEF
#define PRODUCT_DEF	1

#include "price.h"

#define PRODUCT_CODE_LENGTH 9
#define PRODUCT_DESC_LENGTH 32
#define MONTHS 12

// the data type for product categories
typedef enum {
	JACKET,
	SHOES,
	SHIRT,
	SWEATER,
	PANTS,
	ACCESSORY,
	UNKNOWN
} category_t;

// the product data type
typedef struct prod_struct {
	char code[PRODUCT_CODE_LENGTH];		// product code, the key field
	char desc[PRODUCT_DESC_LENGTH];		// product description
	price_t price;						// price per unit
	category_t category;				// product category
	int stock;							// current stock on hand
	int sales[MONTHS];					// sales in each of the last 12 months
	int sales_updated;					// used to detect 0 sales in the new month
	long next;							// hashfile offset of next prodict_t item
} product_t, *product_ptr;

// create and return a new product_t data item from the parameters
product_t new_product(char *code, 
					  char *desc, 
					  price_t price,
					  category_t category,
					  int stock,
					  int sales[MONTHS]);

// display product details to file fp, preceded by before, followed by after
void write_product(FILE *fp, char *before, product_t product, char *after);

// display product category to file fp, preceded by before, followed by after
void write_category(FILE *fp, char *before, category_t category, char *after);

// parse line from inventory file to extract product data fields, create product_t p
int read_product(char *line, product_ptr p);

// parse new product line from transaction file
// to extract product data fields, create product_t p
// there are no sales or stock figures
int read_new_product(char *line, product_ptr p);


#endif