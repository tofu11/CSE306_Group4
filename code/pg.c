#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
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
char *trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}

// Function to clean the last field value
char *clean_last_value(char *str) {
    size_t len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';  // Remove the newline character at the end
    }
    return trim_whitespace(str);
}

void records(int argc, char *argv[], bool header) {
    int target_column = -1;

	FILE *inFile = NULL;
    int field_count;

    // Open file for reading
    inFile = fopen(argv[argc-1], "r");
    if (inFile == NULL) {
        perror("File opening failed");
        return;
    }

    // Skip the first line (header) if 'header' is true
    if (header) {
        char header_line[1024];  // Buffer to store the header line
        if (fgets(header_line, sizeof(header_line), inFile) == NULL) {  // Read the header line and discard it
            printf("Error reading the header.\n");
            fclose(inFile);
            return;
        }
    }

    field_count = F_counter(inFile) - 1;
    rewind(inFile);

    if (field_count == -1) {
        printf("Error reading the file header.\n");
        fclose(inFile);
        return;
    }

    char line[1024];
    char original_line[1024];

    // Read the first line (header) to identify the column index of the requested field
    if (fgets(line, sizeof(line), inFile)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcpy(original_line, line);
        char *field = strtok(line, ",");
        int current_column = 0;

        // Find the index of the target column
        while (field != NULL) {
            field = trim_whitespace(field);
            if (strcmp(field, argv[3]) == 0) {
                target_column = current_column;
                break;
            }
            field = strtok(NULL, ",");
            current_column++;
        }

        if (target_column == -1) {
            printf("Column '%s' not found.\n", argv[3]);
            fclose(inFile);
            return;
        }
    }

    // Now read and process the data
    while (fgets(line, sizeof(line), inFile)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcpy(original_line, line);  // Preserve the original line
        char *field = strtok(line, ",");
        int current_column = 0;
        bool match = false;

        // Traverse fields in the line
        while (field != NULL) {
            // Handle the last field differently if it's the target column
            if (current_column == target_column + 2) {  // Check for the correct column
                if (strtok(NULL, ",") == NULL) {  // If it's the last field in the row
                    field = clean_last_value(field);  // Clean up the last field
                }
                // Compare the target column's value to argv[4]
                if (strcmp(field, argv[4]) == 0) {
                    match = true;
                }
                break;
            }
            field = strtok(NULL, ",");
            current_column++;
        }

        // If a match was found, print the entire original line
        if (match) {
            printf("%s\n", original_line);
        }
    }

    fclose(inFile);
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
    else if(strcmp(argv[1],"-records")==0){
            records(argc, argv, false);
        }
    return 0;
}

