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
        if (header) {
            header = false;
            num -= 1;
            totalSum -= atof(ptr);
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

char *trim_whitespace(char *str) { // trim leading spaces
    char *end;
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // all spaces?
        return str; // return

    // trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // put back new null terminator
    *(end + 1) = '\0';

    return str;
}

// handle strings in quotes
char *parse_quoted_field(char *str, char **end) {
    char *result = malloc(strlen(str) + 1);
    int i = 0;
    str++;  // skip the opening quote using ptr math

    while (*str != '\0' && *str != '"') {
        result[i++] = *str++; // copy over to result
    }
    result[i] = '\0';  // null-termination

    if (*str == '"') {
        str++;  // skip the closing quote
    }

    if (*str == ',') {
        str++;  // skip the comma
    }

    *end = str;  // update the pointer
    return result;
}

bool isNumber(char *input){
    int dot = 0;
    int dash = 0;
    for (int i = 0; input[i] != '\0'; i++) {
            if(input[i]>='A' && input[i]<='Z'|| input[i]>='a' && input[i]<='z'){
                return false;
            }
            if(input[i]=='.'){
                dot++;
            }
            else if(input[i]=='-'){
                dash++;
            }
            else if(input[i] < '0' || input[i] > '9'){
            }else{
                return false;
            }
            
        }
        if(dot>1 || dash>1){
            return false;
        }
        
        return true;
}
void records(int argc, char *argv[], bool header) {
    int target_column = -1;

    FILE *inFile = NULL;
    int field_count;

    // open file for reading
    inFile = fopen(argv[argc-1], "r");
    if (inFile == NULL) {
        perror("File opening failed");
        return;
    }

    // skip the first line (header) if 'header' is true
    if (header) {
        char header_line[1024];  // store the header line
        if (fgets(header_line, sizeof(header_line), inFile) == NULL) {  // Read the header line and discard it
            printf("Error reading the header.\n");
            fclose(inFile);
            return;
        }
    }

    int value;
    if (header) {  // since there are differences in how to handle different lines
        value = 3;
    } else {
        value = 2;
    }

    if (argv[value][0] >= '0' && argv[value][0] <= '9') { // is it number
        field_count = atoi(argv[value]);  // convert to integer if it’s a number
    } else {
        field_count = F_counter(inFile) - 1;  // if not a number, use F_counter and make it a number
    }
    rewind(inFile); // go back inFile

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
        int currIndex = 0;
        while (field != NULL) {
            field = trim_whitespace(field);  // Ensure no trailing or leading whitespace
            if (strcmp(field, argv[value]) == 0) {  // Catch the case if the column is given by string
                target_column = current_column;
                break;
            } else if (currIndex == field_count) {  // Catch the case if the column is given by number
                target_column = current_column;
                break;
            }
            field = strtok(NULL, ",");
            current_column++;
            currIndex++;
        }
        if (target_column == -1) {
            printf("Column '%s' not found.\n", argv[value]);
            fclose(inFile);
            return;
        }
    }

    while (fgets(line, sizeof(line), inFile)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcpy(original_line, line);  // preserve the original line
        char *field = line;
        int current_column = 0;
        bool match = false;

        char accumulated_field[1024] = "";  // init the buffer to accumulate fields
        int secondNum;  // index for the value that we're comparing against

        if (header) { // treat it differently if theres a header or not
            secondNum = 4;
        } else {
            secondNum = 3;
        }

        while (*field != '\0') {
            if (*field == '"') {
                char *end;
                char *quoted_field = parse_quoted_field(field, &end);
                field = end;  // move the pointer to after the quoted field
                if (current_column == target_column) {
                    strcpy(accumulated_field, quoted_field);
                    free(quoted_field);
                    break;
                }
                free(quoted_field);
            } else {
                // Parse unquoted field
                char *unquoted_field = strtok(field, ",");
                field += strlen(unquoted_field) + 1;
                if (current_column == target_column) {
                    strcpy(accumulated_field, unquoted_field);
                    break;
                }
            }
            current_column++;
        }

        char *trimmed_accumulated = trim_whitespace(accumulated_field);
        
        // compare the value
        int trimmed_accumulated_is_num = true;


        if (strcmp(trimmed_accumulated, argv[secondNum]) == 0) {
            match = true;
        }
        else{
        
        if (atof(trimmed_accumulated) - atof(argv[secondNum]) == 0 && isNumber(trimmed_accumulated) && isNumber(argv[secondNum])) {
            match = true;
        }
        }

        // If a match was found, print the entire original line
        if (match) {
            printf("%s\n", original_line);
        }
        current_column = 0;
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

