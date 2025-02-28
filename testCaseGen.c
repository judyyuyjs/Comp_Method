#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char**argv) {
    // LP size (m = # of constraints)
    int LPsize[11] = {100, 200, 300, 500, 600, 800, 1000, 1500, 2000, 3000, 4000};

    srand(time(NULL));
    for(int i = 0; i < 11; i++) {
        char filename[20];
        char filenameAns[20];
        snprintf(filename, 20, "./test_%d.txt", LPsize[i]);
        snprintf(filenameAns, 20, "./test_ans_%d.txt", LPsize[i]);
        FILE *ptr;
        FILE *aptr;
        ptr = fopen(filename, "w");
        aptr = fopen(filenameAns, "w");
        if (ptr == NULL) {
            printf("Error: test file creation\n");
            exit(1);
        }
        int var = rand() % 30; // n - m
        double *ans = (double *) malloc(sizeof(double) * (LPsize[i]+var));
        fprintf(aptr, "[%d] %d ans=", LPsize[i], LPsize[i]+var);
        for(int v = 0; v < LPsize[i]+var; v++) {
            int num = rand() % 10000 + 1;
            ans[v] = (double)num/10000.0;
            fprintf(aptr, "%g ", ans[v]);
        }
        for(int s = 0; s < LPsize[i]; s++){
            // coefficients & RHS (last column)
            double rhs = 0;
            for(int v = 0; v < LPsize[i]+var; v++) {
                int num = rand() % 10000 + 1;
                fprintf(ptr, "%d.%d ", num/10000, num%10000);
                rhs += ((double)num/10000.0) * ans[v];
            }
            fprintf(ptr, "%f\n", rhs);
        }
        // objective function (last row)
        for(int s = 0; s < LPsize[i]+var; s++) {
            int num = rand() % 10000 + 1;
            fprintf(ptr, "%d.%d ", num/10000, num%10000);        
        }
        fprintf(ptr, "\n");
        fclose(ptr);   
    }
    return 0;
}