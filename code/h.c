#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helperfunc.h"
int _h(const char *file) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        return -1;
    }

    char row[1024];
    int count = 0;

    // count numbers
    while (fgets(row, sizeof(row), f)) {
        count++;
    }

    // first row as header
    count--;

    fclose(f);
    return count;
}