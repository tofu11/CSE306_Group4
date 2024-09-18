#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int F_counter(FILE *inFile) {
    char buffer[1000];
    char *data;
    int counter = 0;
    // Read the first line from the file
    if (fgets(buffer, sizeof(buffer), inFile) != NULL) {
        data = strtok(buffer, ",");
        while (data != NULL) {
            counter++;
            data = strtok(NULL, ",");
        }
    } else {
        return -1; 
    }
    return counter;
}
int main(int argc, char *argv[]) {
    FILE * inFile = NULL;
    int fcount;
    inFile = fopen(argv[argc-1], "r");  // open file for reading
    if (inFile == NULL) {
      return -1;
}
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) { 
            fcount = count_fields(inFile);
            printf("%d\n", fcount);
        }
    }
    fclose(inFile);
    return 0;
}