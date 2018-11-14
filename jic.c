/*
* Module to tokenize an array of strings based on an array of delimiters.
* Author: Simon Haile
* Date: May 22, 2017
*/
//#include "tokenizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int tokenize(char *line, char *delimiters);

int main(int argc, char *argv[]){

  char delimiters[2]  = {'-', '!'};
  tokenize("hello-word!", delimiters);
}
/*
* Loop through the line, until the null character, to determine size
* of array to return.
*/
int tokenize(char *line, char *delimiters){
  int arrSize=0;
  while(*line != '\0'){
    printf(":%c\n",*line );
    //while i havent found a token counterr, keep going.
    //add one to arrSize
    //while i havent found a delimiter, keep going.
      if((strchr(delimiters, *line)) || (*line == '\0')){
        line++;
        if( ((!(strchr(delimiters, *line))) && (*line != '\n')) ){
          line--;
          arrSize++;
            printf("%d\n",arrSize );
        }
        else{
          line--;
        }
      }
      ++line;
  }
  return arrSize;
}
