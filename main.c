#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_r_h.h"


int main(int argc, char *argv[]) {
    int h = 0; 
    int count = 0; 
    char *file = NULL;
    int r = 0;
    int min = 0;
    int max = 0;
    int mean = 0;
    int records = 0;

    // check option
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            h = 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            r = 1;  
        } else if (strcmp(argv[i], "-max") == 0) {
            max = 1;
        } else if (strcmp(argv[i], "-min") == 0) {
            min = 1;
        } else if (strcmp(argv[i], "-mean") == 0) {
            mean = 1;
        } else if (strcmp(argv[i], "-records") == 0) {
            records = 1;
        }else {
            file = argv[argc - 1];
        }
    }

    // check if -h is used alone
    if (h == 1){
        if(r == 0 && max == 0 && min == 0 && mean == 0 && records == 0){
            return EXIT_FAILURE;
        }
    }

    // check file
    if (file == NULL) {
        return EXIT_FAILURE;
    }

    // -r or -h
    if (h) {
        count = _h(file);
    } else {
        count = _r(file);
    }

    //print numbers
    if(count == -1){
        return EXIT_FAILURE;
    }else{
        printf("%d\n", count);
    }
    return EXIT_SUCCESS;
}