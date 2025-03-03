#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTCASES 6

int main(int argc, char**argv) {
    // LP size (m = # of constraints)
    //int LPsize[TESTCASES] = {100, 200, 300, 500, 600, 800, 1000, 1500, 2000, 3000, 4000};
    int LPsize[TESTCASES] = {10, 25, 50, 75, 100, 150};

    srand(time(NULL));
    for(int i = 0; i < TESTCASES; i++) {
        char filename[20];
        snprintf(filename, 20, "./test_%d.txt", LPsize[i]);
        FILE *ptr;
        ptr = fopen(filename, "w");
        if (ptr == NULL) {
            printf("Error: test file creation\n");
            exit(1);
        }
        
        char filenameAns[20];
        snprintf(filenameAns, 20, "./test_ans_%d.txt", LPsize[i]);
        FILE *aptr;
        aptr = fopen(filenameAns, "w");
        if (aptr == NULL) {
            printf("Error: test file creation (ans)\n");
            exit(1);
        }

        int var = rand() % LPsize[i] + LPsize[i]; // n - m
        double *ans = (double *) malloc(sizeof(double) * (LPsize[i]+var));
        fprintf(aptr, "[%d] %d ans=", LPsize[i], LPsize[i]+var);
        for(int v = 0; v < LPsize[i]+var; v++) {
            int num = rand() % 10000 + 1;
            ans[v] = (double)num/10000.0;
            fprintf(aptr, "%g ", ans[v]);
        }
        int j = 0;
        for(int s = 0; s < LPsize[i]; s++){
            // coefficients & RHS (last column)
            double rhs = 0;
            int v = 0;
            for(; v < var; v++) {
                int num = rand() % 10000 + 1;
                fprintf(ptr, "%d.%d ", num/10000, num%10000);
                rhs += ((double)num/10000.0) * ans[v];
            }
            for(; v < LPsize[i]+var; v++) {
                if(j + var == v) {
                    fprintf(ptr, "-1 ");
                    rhs += ans[v];
                } else {
                    fprintf(ptr, "0 ");
                }
            }
            fprintf(ptr, "%f\n", rhs);
            j += 1;
        }
        // objective function (last row)
        int s = 0;
        for(; s < var; s++) {
            int num = rand() % 10000 + 1;
            fprintf(ptr, "%d.%d ", num/10000, num%10000);        
        }
        for(; s < LPsize[i]+var; s++) {
            fprintf(ptr, "0 ");
        }
        fprintf(ptr, "\n");
        
        fclose(ptr);
        fclose(aptr);
    }
    return 0;
}