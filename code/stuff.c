#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE * inFile = NULL;
  
    inFile = fopen(argv[argc-1], "r");  // open file for reading
    if (inFile == NULL) {
      printf("Error opening input file\n");
      return -1;
    }
    if (inFile == NULL) {
    exit(EXIT_FAILURE);
  }
  }