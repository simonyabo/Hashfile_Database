/*
 * price.h
 * 
 * header file for inventory price data item
 * 
 * CSCI 352, Spring 2017, Assignment 3
 * 
 * David Bover, May 2017
 * 
 */

#ifndef PRICE_DEF
#define PRICE_DEF	1

#define NEWLINE	1
#include <stdio.h>

typedef struct price_struct {
	int negative;
	unsigned int dollars;
	unsigned int cents;
} price_t, *price_ptr;

// display the price to file fp, preceded by before, followed by after
void write_price(FILE *fp, char *before, price_t p, char *after);

// create and return a price value from the parameters
price_t new_price(int dollars, int cents, int negative);

// return the sum of prices: p1 + p2
price_t price_add(price_t p1, price_t p2);

// return the difference of prices: p1 - p2
price_t price_subtract(price_t p1, price_t p2);

// return a price being the total cost of quantity of items each at price p
price_t price_multiply(price_t p, int quantity);

// compare two prices
// return <0 if p1<p2, 0 if p1==p2, >0 if p1>p2
int compare(price_t p1, price_t p2);


#endif
