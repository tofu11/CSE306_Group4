#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  FILE * inFile = NULL;

  printf("argc: %d\n",argc);

  // remember: program name is argv[0]
  if (argc == 2) {
    inFile = fopen(argv[1], "r");  // open file for reading
    if (inFile == NULL) {
      printf("Error opening input file\n");
    }
  }
  else {
    printf("Incorrect number of command line arguments.\n");
  }

  if (inFile == NULL) {
    exit(EXIT_FAILURE);
  }