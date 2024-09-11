#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void for_r_h(const char* filename, int records, int header) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int count = 0;

    // Count the number of records
    while (fgets(line, sizeof(line), f)) {
        count++;
    }

    // check if -h
    if (header == 0) {
        count++;
    }

    // -r
    if (records) {
        if(header == 1){
            printf("%d\n", count-1);
        }else{
            printf("%d\n", count);
        }    
    }

    fclose(f);
}

int main(int argc, char *argv[]) {
    int records = 0;
    int header = 0;
    char *file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            records = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            header = 1;
        } else {
            file = argv[i];
        }
    }

    for_r_h(file, records, header);

    return EXIT_SUCCESS;
}