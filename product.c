/*
 * product.c
 * 
 * functions for inventory product data item
 * 
 * CSCI 352, Spring 2017, Assignment 3
 * 
 * David Bover, May 2017
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "product.h"
#include "tokenizer.h"

// create and return a new product_t data item from the parameters
product_t new_product(char *code, 
					  char *desc, 
					  price_t price,
					  category_t category,
					  int stock,
					  int sales[MONTHS]) {
	
	product_t product;
	strncpy(product.code, code, PRODUCT_CODE_LENGTH - 1);
	strncpy(product.desc, desc, PRODUCT_DESC_LENGTH - 1);
	product.price = price;
	product.category = category;
	product.stock = stock;
	int i;
	for (i=0; i<MONTHS; i++)
		product.sales[i] = sales[i];
	product.sales_updated = 0;
	product.next = 0;
	return product;
}

// display product details to file fp, preceded by before, followed by after
void write_product(FILE *fp, char *before, product_t product, char *after) {
	fprintf(fp, "%s", before);
	fprintf(fp, "%s: '%s' ", product.code, product.desc);
	write_price(fp, "", product.price, "");
	write_category(fp, " ", product.category, "");
	fprintf(fp, " %d", product.stock);
	int i;
	fprintf(fp, " (%d", product.sales[0]);
	for (i=1; i<MONTHS; i++)
		fprintf(fp, " %d", product.sales[i]);
	fprintf(fp, ") next:%d", (int)product.next);
	fprintf(fp, "%s", after);
}

// display product category to file fp, preceded by before, followed by after
void write_category(FILE *fp, char *before, category_t category, char *after) {
	fprintf(fp, "%s", before);
	switch(category) {
		case JACKET:    fprintf(fp, "Jacket");    break;
		case SHOES:     fprintf(fp, "Shoes");     break;
		case SHIRT:     fprintf(fp, "Shirt");     break;
		case SWEATER:   fprintf(fp, "Sweater");   break;
		case PANTS:     fprintf(fp, "Pants");     break;
		case ACCESSORY: fprintf(fp, "Accessory"); break;
		default:        fprintf(fp, "Unknown");   break;		
	}
	fprintf(fp, "%s", after);
}

// internal function to determine category type from a string
static category_t category_from_string(char *name) {
	if (strcasecmp(name, "JACKET") == 0)
		return JACKET;
	if (strcasecmp(name, "SHOES") == 0)
		return SHOES;
	if (strcasecmp(name, "SHIRT") == 0)
		return SHIRT;
	if (strcasecmp(name, "SWEATER") == 0)
		return SWEATER;
	if (strcasecmp(name, "PANTS") == 0)
		return PANTS;
	if (strcasecmp(name, "ACCESSORY") == 0)
		return ACCESSORY;
	return UNKNOWN;
}

// parse new product line from transaction file
// to extract product data fields, create product_t p
// there are no sales or stock figures
int read_new_product(char *line, product_ptr product){
	
	char *c;
	char **tokens;
	int dollars, cents;
	int i;
		
	// get the product code
	c = &(line[0]);
	strncpy(product->code, c, PRODUCT_CODE_LENGTH-1);
	product->code[PRODUCT_CODE_LENGTH-1] = '\0';
	
	// get the product description
	c += PRODUCT_CODE_LENGTH - 1;
	strncpy(product->desc, c, PRODUCT_DESC_LENGTH-1);
	product->desc[PRODUCT_DESC_LENGTH-1] = '\0';
	
	// tokenize the rest of the line
	c += PRODUCT_DESC_LENGTH - 1;
	tokens = tokenize(c, " ");
	if (token_count(tokens) < 2) return 0;
	
	// get the price dollars and cents
	if (sscanf(tokens[0], "%d.%d", &dollars, &cents) < 2) return 0;
	product->price = new_price(dollars, cents, 0);
	
	// get the category, stock and sales for the past MONTHS
	product->category = category_from_string(tokens[1]);
	
	// initialize other fields to zero
	product->stock = 0;
	for (i=0; i<MONTHS; i++)
		product->sales[i] = 0;

	product->sales_updated = 0;
	product->next = 0;
	
	return 1;
}

// parse line from inventory file to extract product data fields, create product_t p
int read_product(char *line, product_ptr product){
	
	char *c;
	char **tokens;
	int dollars, cents;
	int i;
	
	// get the product code	
	c = &(line[0]);
	strncpy(product->code, c, PRODUCT_CODE_LENGTH-1);
	product->code[PRODUCT_CODE_LENGTH-1] = '\0';
	
	// get the product description
	c += PRODUCT_CODE_LENGTH - 1;
	strncpy(product->desc, c, PRODUCT_DESC_LENGTH-1);
	product->desc[PRODUCT_DESC_LENGTH-1] = '\0';
	
	// tokenize the rest of the line
	c += PRODUCT_DESC_LENGTH - 1;
	tokens = tokenize(c, " ");
	if (token_count(tokens) < 15) return 0;
	
	// get the price dollars and cents
	if (sscanf(tokens[0], "%d.%d", &dollars, &cents) < 2) return 0;
	product->price = new_price(dollars, cents, 0);
	
	// get the category, stock and sales for the past MONTHS
	product->category = category_from_string(tokens[1]);

	// first check that the remaining tokens are all integers
	for (i = 2; i <= 2+MONTHS; i++)
		if (strspn(tokens[i], "0123456789") != strlen(tokens[i])) return 0;
		
	product->stock = atoi(tokens[2]);
	
	for (i=0; i<MONTHS; i++){
		product->sales[i] = atoi(tokens[3+i]);
	}
	// initialize other fields to zero
	product->sales_updated = 0;
	product->next = 0;
	
	return 1;
}
