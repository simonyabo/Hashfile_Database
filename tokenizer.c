/*
* Module to tokenize an array of strings based on an array of delimiters.
* Author: Simon Haile
* Date: May 22, 2017
*/
//#include "tokenizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#define MAX_TOK_LEN 1024
#define PRODUCT_CODE_LENGTH 9
#define PRODUCT_DESC_LENGTH 32
#define MONTHS 12


char **tokenize(char *line, char *delimiters);


char ** tokenize(char *line, char *delimiters){
  int arrSize=0;
  char *inputLine = line;
  while((*line != '\0') ){
      while((strchr(delimiters, *line)) && (*line != '\0')  ) //while i havent found a token, keep going.
        line++;
      if((!strchr(delimiters, *line)) && (*line != '\0'))
          arrSize++; //add one to arrSize
      while(!(strchr(delimiters, *line)) ) //while i havent found a delimiter, keep going.
        line++;
  }
  line = inputLine;
  char *word = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
  char **p = (char **) malloc(sizeof(char *) * ( arrSize +1));
  int i, j =0;
    while(*line != '\0'){
        i=0;
        while((strchr(delimiters, *line)) && (*line != '\0')  ){
          line++;
        }
        while(!(strchr(delimiters, *line))  ){
          word[i] = *line++;
          i++;
        }
		word[i] = '\0';
        p[j] = (char *)malloc(strlen(word) +1);
        strncpy(p[j], word, (strlen(word)+1));
        memset(word, 0, MAX_TOK_LEN);
        j++;
        p[j] = NULL;
    }


  return p;
}

int token_count(char **tokens){
 	int count = 0;
	for(int j=0; tokens[j]; j++){
		count++;
   	}

return count;
}
