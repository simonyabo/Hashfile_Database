/*
 *	tokenizer.h
 * 
 * 	header file for a string tokenizer
 * 
 * 	David Bover, May 2017
 */
 
#ifndef TOKENIZER_DEF
#define TOKENIZER_DEF 1

// break up the line into individual tokens, based on delimiters
// returns a null-terminated array of strings
char **tokenize(char *line, char *delimiters);

// returns a count of the number of strings in the null-terminated
// array (excluding the null element)
int token_count(char **tokens);

#endif