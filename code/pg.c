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
    *(end + 1) = '\0';

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

// Function to handle quoted fields in CSV
char *parse_quoted_field(char *str, char **end) {
    char *result = malloc(strlen(str) + 1);
    int i = 0;
    str++;  // Skip the opening quote

    while (*str != '\0' && *str != '"') {
        result[i++] = *str++;
    }
    result[i] = '\0';  // Null-terminate the result

    if (*str == '"') {
        str++;  // Skip the closing quote
    }

    if (*str == ',') {
        str++;  // Skip the comma following the quoted field
    }

    *end = str;  // Update the pointer to the end of the quoted field
    return result;
}

// Concatenate the fields until we hit a delimiter or end of field
void accumulate_field(char *accumulated, const char *field) {
    if (strlen(accumulated) > 0) {
        strcat(accumulated, ", ");
    }
    strcat(accumulated, field);
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

    int value;
    if (header) {  // since there are differences
        value = 3;
    } else {
        value = 2;
    }

    if (argv[value][0] >= '0' && argv[value][0] <= '9') {
        field_count = atoi(argv[value]);  // Convert to integer if it’s a number
    } else {
        field_count = F_counter(inFile) - 1;  // If not a number, use F_counter
    }
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
        int currIndex = 0;
        while (field != NULL) {
            field = trim_whitespace(field);
            if (strcmp(field, argv[value]) == 0) {  // Catch the case if the column is given by string
                target_column = current_column;
                break;
            }
            else if (currIndex == field_count) {  // Catch the case if the column is given by number
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

    // Now read and process the data
    while (fgets(line, sizeof(line), inFile)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        strcpy(original_line, line);  // Preserve the original line
        char *field = line;
        int current_column = 0;
        bool match = false;

        char accumulated_field[1024] = "";  // Initialize the buffer to accumulate fields
        int secondNum;  // Index for the value that we're comparing against

        if (header) {
            secondNum = 4;
        } else {
            secondNum = 3;
        }

        // Traverse fields in the line
        while (*field != '\0') {
            if (*field == '"') {
                // Parse quoted field
                char *end;
                char *quoted_field = parse_quoted_field(field, &end);
                field = end;  // Move the pointer to after the quoted field
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

//        printf("Accumulated value: '%s'\n", accumulated_field);
  //      printf("Comparing to: '%s'\n", argv[secondNum]);

        // Compare the full accumulated value
        if (strcmp(accumulated_field, argv[secondNum]) == 0) {
            match = true;
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

