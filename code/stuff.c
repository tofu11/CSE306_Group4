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

int F_counter(FILE *inFile) {
    char buffer[1000];
    char *data;
    int counter = 0;

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
void records(int argc, char *argv[], bool header) {
    int index = -1;

    if (header) {
        // Check if argv[3] is a number and convert it using atoi if it is
        if (argv[3][0] >= '0' && argv[3][0] <= '9') {
            index = atoi(argv[3]);
            printf("%d\n", index); // Print the converted integer
        } else {
                FILE * inFile = NULL;
                int fcount;
                // open file for reading
                inFile = fopen(argv[argc-1], "r");
                if (inFile == NULL) {
                        return -1;
                }
                //conditional for seeing which helper function to use
            fcount = F_counter(inFile)-1;
            printf("%d\n", fcount);

            char line[1024];
            char **list = NULL;  // Pointer to dynamically allocated array of strings
            int line_count = 0;

            while (fgets(line, sizeof(line), inFile)) {
            // Dynamically grow the array to accommodate a new line
            list = realloc(list, (line_count + 1) * sizeof(char *));
            if (list == NULL) {
                perror("Memory allocation failed");
                return EXIT_FAILURE;
            }

            // Allocate memory for the line and copy the line's content
            list[line_count] = malloc(strlen(line) + 1);
            strcpy(list[line_count], line);
            line_count++;
        }
            for (int i = 0; i < line_count; i++) {
        // Parse the line into fields by splitting with ','
            char *field = strtok(list[i], ",");
            int field_count = 1;
            bool match = false;
            while (field != NULL) {
                if(field_count==3){
                    if(strcmp(field,argv[4])==0){
                        match=true;
                        //printf("%s",list[line_count]);
                        //printf("  Field %d: %s\n", field_count, field);
                    }
                    
                }
                if(match){
                    printf("%s\n", list[i]);
                    match=false;
                }
                
                field = strtok(NULL, ",");
                field_count++;
                
            }
    }

            fclose(inFile);

        }
            }
    return 0;


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

