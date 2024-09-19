#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helperfunc.h"

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