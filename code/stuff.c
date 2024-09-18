#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE * inFile = NULL;
    char buffer[1000];
    char *data;
    int counter=0;

    inFile = fopen(argv[argc-1], "r");  // open file for reading
    if (inFile == NULL) {
      return -1;
    }
      if (fgets(buffer, sizeof(buffer), inFile) != NULL) {
      data = strtok(buffer, ",");
      while (data != NULL) {
          counter++;
          data = strtok(NULL, ",");
      }
  }
        else{
        fclose(inFile);
        return -1;
      }
    printf("%d\n", counter);
    fclose(inFile);
    return 0;
}

