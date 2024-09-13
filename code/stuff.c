#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float meanField(int argc, char *argv[]) {
    FILE *file = fopen(argv[argc-1], "r"); // Open the CSV file passed as the last argument
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
	int num = 0;
	int totalSum = 0;
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
	num+=1;
        totalSum+=atoi(ptr);
	
    }

    // Close the file
    fclose(file);
    printf("Total%d\n",totalSum);
    printf("Total num%d\n",num);
    printf("%d\n",totalSum/num);
    return totalSum/num;
}


int main(int argc,char *argv[]){
	meanField(argc, argv);
    
}
