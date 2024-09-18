#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

float meanField(int argc, char *argv[], bool header) {
    FILE *file = fopen(argv[argc-1], "r"); // Open the CSV file passed as the last argument
    if (file == NULL) {
        perror("Error opening file");
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
        if(header){
        header = false;
        num-=1;
        totalSum-=atof(ptr);
        }
        num += 1;
        totalSum += atof(ptr); // Use atof() to handle floating-point values
    }

    // Close the file
    fclose(file);

    if (num == 0) {
        printf("No numeric data found.\n");
        return 1;
    }

    float mean = totalSum / num;
    return mean;
}
void records(int argc, char *argv[], bool header) {
    int index = -1;

    if (header) {
        // Check if argv[3] is a number and convert it using atoi if it is
        if (argv[3][0] >= '0' && argv[3][0] <= '9') {
            index = atoi(argv[3]);
            printf("%d\n", index); // Print the converted integer
        } else {
            }

            fclose(file);
	if (index != -1) {
                printf("Field '%s' found at index %d\n", field, index);
            } else {
                printf("Field '%s' not found in header.\n", field);
            }    
    }
}
int main(int argc, char *argv[]) {
    float meanVal = 0.0;
    if(strcmp(argv[1],"-h")==0){
        if(strcmp(argv[2],"-mean")==0){
            meanVal = meanField(argc, argv, true);
            printf("%0.2f\n",meanVal);
        }
        if(strcmp(argv[2],"-records")==0){
            records(argc, argv, true);
        }
    }
    else if(strcmp(argv[1],"-mean")==0){
            meanVal = meanField(argc, argv, false);
            printf("%0.2f\n",meanVal);
        }
    else if(strcmp(argv[2],"-records")==0){
            records(argc, argv, false);
        }
    return 0;
}

