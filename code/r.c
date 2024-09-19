#include <stdio.h>
#include <stdlib.h>
#include "helperfunc.h"


int _r(const char *file) {
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

    fclose(f);
    return count;
}