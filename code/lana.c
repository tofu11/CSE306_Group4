#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

float minField(int argc, char *argv[], int field) {
    FILE *inFile = fopen(argv[argc - 1], "r"); 
    if (inFile == NULL) {
        return EXIT_FAILURE;
    }

    int num = 0;
    float minValue = FLT_MAX; 
    char row[1024]; 

    // Skip header if needed
    if (strcmp(argv[1], "-h") == 0) {
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
    if (num == 0){
        return -1;
    }
    return minValue; 
}

float maxField(int argc, char *argv[], int field) {
    FILE *inFile = fopen(argv[argc - 1], "r"); 
    if (inFile == NULL) {
        return EXIT_FAILURE; 
    }

    int num = 0;
    float maxValue = FLT_MIN;
    char row[1024]; 

    if (strcmp(argv[1], "-h") == 0) {
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
    if (num == 0){
        return -1;
    }
    return maxValue;
}

float meanField(int argc, char *argv[], bool header) {
    FILE *file = fopen(argv[argc-1], "r"); // Open the CSV file passed as the last argument
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }
    int num = 0;
    float totalSum = 0.0;
    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Find the length of the line
        size_t len = strlen(line);

        // Remove newline character if present
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--; // Adjust length after removing newline
        }

        // Use a pointer to traverse from the end of the line backwards
        char *ptr = line + len - 1;

        // Move pointer backwards until the last comma is found
        while (ptr > line && *ptr != ',') {
            ptr--;
        }

        // Print the value after the last comma
        if (*ptr == ',') {
            ptr++; // Move pointer to the first character after the comma
        }
        /*
        if (header) {
            header = false;
            num -= 1;
            totalSum -= atof(ptr);
        }
        */
        num += 1;
        totalSum += atof(ptr); // Use atof() to handle floating-point values
    }

    // Close the file
    fclose(file);

    if (num == 0) {
        printf("No numeric data found.\n");
        return 1;
    }

    num-=1;

    float mean = totalSum / num;
    return mean;
}

int main(int argc, char *argv[]) {
    bool header = false;
    if(strcmp(argv[1], "-h")==0){
        header=true;
    }
    int fieldIndex;

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-min")==0){
            float minValue = minField(argc, argv, atoi(argv[i+1]));
            if (minValue == -1) {
            return EXIT_FAILURE;
        } else {
            printf("%.0f\n", minValue);
        }
        }
        else if(strcmp(argv[i], "-max")==0){
            float maxValue = maxField(argc, argv, atoi(argv[i+1]));
            if (maxValue == -1) {
                return EXIT_FAILURE;
            } else {
                printf("%.0f\n", maxValue);
            }
        }
        else if(strcmp(argv[i], "-mean")==0){
            float meanValue = meanField(argc, argv, header);
            if (meanValue == -1) {
                return EXIT_FAILURE;
            } else {
                printf("%.2f\n", meanValue);
            }
        }
    }

    return EXIT_SUCCESS;
}