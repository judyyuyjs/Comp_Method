#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readTestfile.h"

int main()
{
    const char* filename = "./test_10.txt";
    double *A, *b, *c;
    int numRows = rowCount(filename);
    int numCols = colCount(filename);
//    printf("%d %d\n", numRows, numCols);
    
    A = (double *) malloc(sizeof(double) * numRows*numCols);
    b = (double *) malloc(sizeof(double) * numRows);
    c = (double *) malloc(sizeof(double) * numCols);
    
    readFile(filename, A, b, c, numRows, numCols);
    
    printf("########### matrix A:\n\t");
    for(int i = 0; i < numRows*numCols; i++){
        printf("%g ", A[i]);
        if((i+1) % numCols == 0){
            printf("\n\t");
        }
    }
    printf("\n########### b:\n\t");
    for(int j = 0; j < numRows; j++){
        printf("%g ", b[j]);
    }
    printf("\n\n########### c:\n\t");
    for(int k = 0; k < numCols; k++){
        printf("%g ", c[k]);
    }
    
    free(A);
    free(b);
    free(c);
    return 0;
}