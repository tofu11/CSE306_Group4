#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For INT_MIN and INT_MAX

int main(int argc, char *argv[]){}

void minMax(FILE*inFile, int field, char mode){
    int comapre;
    int maxNum = INT_MAX;
    int minNum = INT_MIN;

    while(fscanf(inFIle, "%d", &compare) == 1){
        if(mode == 'm'){
            (if compare>maxNum){
                maxNum = compare;
            }
        }else if(if mdoe == 'n'){
            if(compare<minNum){
                minNum = compare;
            }
        }
    }

    if(mode == "m"){
        printf("Maximum: %d\n" maxNum);
    }else if (mode == 'n'){
        printf("Minimum: %d\n minNum");
    }
}

    

    //file open for 

    //access arguments here

    // initalize variables here


    // find min and max here 

}

