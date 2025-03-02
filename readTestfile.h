#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

int rowCount(const char* filename){
    FILE *fptr = fopen(filename, "r");
    
    char buff[1024];
    int numRow = 0;
    while(fgets(buff, sizeof(buff), fptr)) {
        numRow++;
    }
    return numRow-1;
}
int colCount(const char* filename){
    FILE *fptr = fopen(filename, "r");
    
    char buff[1024];
    const char* tok;
    int numCol = 0;
    fgets(buff, sizeof(buff), fptr);
    for(tok = strtok(buff, " "); tok && *tok; tok = strtok(NULL, " \n")) {
        numCol++;
    }
    return numCol-1;
}


int readFile(const char* filename, double *A, double *b, double *c, int numRow, int numCol) {
    FILE *fptr = fopen(filename, "r");
    
    char buff[1024];
    const char* tok;
    double *matrix = (double *) malloc(sizeof(double) * (numRow+1)*(numCol+1));
    int i = 0;
    while(fgets(buff, sizeof(buff), fptr)) {
        for(tok = strtok(buff, " "); tok && *tok; tok = strtok(NULL, " \n"))
        {
            matrix[i] = atof(tok);
            i++;
        }
    }
    int A_index = 0, b_index = 0, c_index = 0;
    for(int j = 0; j < (numRow+1)*(numCol+1)-1; j++){
        if((j+1) % (numCol+1) == 0) {
            b[b_index] = matrix[j];
            b_index++;
        } else if (j < numRow*(numCol+1)){
            A[A_index] = matrix[j];
            A_index++;
        } else {
            c[c_index] = matrix[j];
            c_index++;
        }
#if DEBUG
        printf("%g ", matrix[j]);
        if((j+1) % (numCol+1) == 0) {
            printf("\n");
        }
#endif
    }

    fclose(fptr);
    free(matrix);
    return 0;
}