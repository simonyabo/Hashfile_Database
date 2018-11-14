/*
 * price.c
 * 
 * functions for inventory price data item
 * 
 * CSCI 352, Spring 2017, Assignment 3
 * 
 * David Bover, May 2017
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "price.h"

// display the price to file fp, preceded by before, followed by after
void write_price(FILE *fp, char *before, price_t price, char *after) {
	fprintf(fp, "%s", before);
	if (price.negative)
		fprintf(fp, "-");
	fprintf(fp, "$%d.%d", price.dollars, price.cents);
	fprintf(fp, "%s", after);
}

// create and return a price value from the parameters
price_t new_price(int dollars, int cents, int negative) {
	price_t price;
	price.negative = negative;
	price.dollars = dollars;
	price.cents = cents;
	return price;
}

// return the sum of prices: p1 + p2
price_t price_add(price_t p1, price_t p2) {
	price_t price;
	int sign1 = (p1.negative)?-1:1;
	int sign2 = (p2.negative)?-1:1;
	int cents1 = sign1 * (100 * p1.dollars + p1.cents);
	int cents2 = sign2 * (100 * p2.dollars + p2.cents);
	int cents3 = cents1 + cents2;
	if (cents3 < 0) {
		price.negative = 1;
		cents3 = -cents3;
	}
	price.dollars = cents3 / 100;
	price.cents = cents3 % 100;
	return price;
}

// return the difference of prices: p1 - p2
price_t price_subtract(price_t p1, price_t p2){
	price_t price;
	int sign1 = (p1.negative)?-1:1;
	int sign2 = (p2.negative)?-1:1;
	int cents1 = sign1 * (100 * p1.dollars + p1.cents);
	int cents2 = sign2 * (100 * p2.dollars + p2.cents);
	int cents3 = cents1 - cents2;
	if (cents3 < 0) {
		price.negative = 1;
		cents3 = -cents3;
	} else
		price.negative = 0;
	price.dollars = cents3 / 100;
	price.cents = cents3 % 100;
	return price;
}

// return a price being the total cost of quantity of items each at price p
price_t price_multiply(price_t p, int quantity){
	price_t price;
	int cents = (100 * p.dollars + p.cents) * quantity;
	price.dollars = cents / 100;
	price.cents = cents % 100;
	price.negative = p.negative;
	return price;
}

// compare two prices
// return <0 if p1<p2, 0 if p1==p2, >0 if p1>p2
int compare(price_t p1, price_t p2) {
	int sign1 = (p1.negative)?-1:1;
	int sign2 = (p2.negative)?-1:1;
	int cents1 = sign1 * (100 * p1.dollars + p1.cents);
	int cents2 = sign2 * (100 * p2.dollars + p2.cents);
	return cents1 - cents2;
}

