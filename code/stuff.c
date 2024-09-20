#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

float minField(int argc, char *argv[], int field) {
  
    bool header = false;
    if (argc > 2 && strcmp(argv[2], "-h") == 0) {
        header = true; 
    }

    FILE *inFile = fopen(argv[argc - 1], "r"); 
    if (inFile == NULL) {
        perror("Error opening file");
        return FLT_MAX; 
    }

    int num = 0;
    float minValue = FLT_MAX; 
    char row[1024]; 

   
    if (header) {
        fgets(row, sizeof(row), inFile); 
    }

    while (fgets(row, sizeof(row), inFile)) {
        
        size_t len = strlen(row);

       
        if (len > 0 && row[len - 1] == '\n') {
            row[len - 1] = '\0'; 
        }

       
        char *ptr = row;
        int currentField = 0;

       
        while (1) {
            char *comma = strchr(ptr, ',');
            if (comma == NULL) {
                comma = row + strlen(row); 
            }

            
            if (currentField == field) {
                char temp[64];
                strncpy(temp, ptr, comma - ptr);
                temp[comma - ptr] = '\0'; 

                char *endptr;
                float value = strtof(temp, &endptr);
                if (*endptr == '\0') { 
                    if (value < minValue) {
                        minValue = value; 
                    }
                    num++;
                }
                break; 
            }

            ptr = comma + 1; 
            currentField++;
            if (*comma == '\0') break; 
        }
    }

    fclose(inFile);

    if (num == 0) {
        return 1;
    }

    return minValue; 
}



float maxField(int argc, char *argv[], int field) {
  
    bool header = false;
    if (argc > 2 && strcmp(argv[2], "-h") == 0) {
        header = true; 
    }

    FILE *inFile = fopen(argv[argc - 1], "r"); 
    if (inFile == NULL) {
        perror("Error opening file");
        return FLT_MIN; 
    }

    int num = 0;
    float maxValue = FLT_MIN;
    char row[1024]; 

    if (header) {
        fgets(row, sizeof(row), inFile); 
    }

    while (fgets(row, sizeof(row), inFile)) {
        
        size_t len = strlen(row);
        
        if (len > 0 && row[len - 1] == '\n') {
            row[len - 1] = '\0'; 
        }

        char *ptr = row;
        int currentField = 0;

        while (1) {
            char *comma = strchr(ptr, ',');
            if (comma == NULL) {
                comma = row + strlen(row); 
            }

            if (currentField == field) {
                char temp[64];
                strncpy(temp, ptr, comma - ptr);
                temp[comma - ptr] = '\0'; 

                char *endptr;
                float value = strtof(temp, &endptr);
                if (*endptr == '\0') { 
                    if (value > maxValue) { 
                        maxValue = value; 
                    }
                    num++;
                }
                break; 
            }

            ptr = comma + 1; 
            currentField++;
            if (*comma == '\0') break; 
        }
    }

    fclose(inFile);

    if (num == 0) {
        return 1; 
    }

    return maxValue; 
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        return EXIT_FAILURE;
    }

    int fieldIndex = atoi(argv[1]);
    if (fieldIndex < 0) {
        return EXIT_FAILURE;
    }

    bool hasHeader = (argc > 3 && strcmp(argv[2], "-h") == 0);
   char *filename;
   if (hasHeader) {
    filename = argv[3];
    } else {
    filename = argv[2];
    }

    float minValue = minField(argc, argv, fieldIndex);
    float maxValue = maxField(argc, argv, fieldIndex);

    if (minValue == FLT_MAX) {
        fprintf(stderr, "Could not find minimum value or file is empty.\n");
    } else {
        printf("Minimum value in field %d: %.2f\n", fieldIndex, minValue);
    }

    if (maxValue == FLT_MIN) {
        fprintf(stderr, "Could not find maximum value or file is empty.\n");
    } else {
        printf("Maximum value in field %d: %.2f\n", fieldIndex, maxValue);
    }

    return EXIT_SUCCESS;
}