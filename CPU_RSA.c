#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrixinverse.h"

// data is LP problem in standard form
// min z = cx
// s.t. Ax = b
//      x >= 0

#define num_row 3
#define num_col 6
#define N 3  // number of basic variable
#define M 3 // number of non basic variable

// Use basic index to find Ab, An
void find_AJ(double A[num_row][num_col], int basis[], double **Aj) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aj[i][j] = A[i][basis[j]];
        }
    }
}

void find_An(double A[num_row][num_col], int basis[], double **Aj) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Aj[i][j] = A[i][basis[j]];
        }
    }
}

int findMin(double arr[], int n) {
    int min = arr[0];
    int min_index = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] < 0){
            if (arr[i] == min){
                min = arr[i+1];
            }
            continue;
        }

        if (arr[i] <= min) {
            min = arr[i];
            min_index = i;
        }
    }
    return min_index;
}

int assign_variable(int basic_index[N], int non_basic_index[M], int entering_var, int leaving_var) {
    for (int i = 0; i < N; i++) {
        if (basic_index[i] == leaving_var) {
            basic_index[i] = entering_var;
            break;
        }
    }

    for (int j = 0; j < M; j++) {
        if (non_basic_index[j] == entering_var) {
            non_basic_index[j] = leaving_var;
            break;    
        }
    }
    return 0;
}


int main(){

    double A[num_row][num_col] = {
    {8, 6, 1, 1, 0, 0},
    {4, 2, 1.5, 0, 1, 0},
    {2, 1.5, 0.5, 0, 0, 1}};
    double b[num_row] = {48, 20, 8};
    double c[num_col] = {-60, -30, -20, 0, 0, 0};
    // initial basis
    int non_basic_index[M];
    for (int i = 0; i < M; i++) {
        non_basic_index[i] = i;
    }

    int basic_index[N];
    for (int i = M; i < num_col; i++) {
        basic_index[i-M] = i;
    }

    int iter_num = 0;
    while (iter_num <= 2000) {
        iter_num++;

        // initialize Ab, An
        double **Ab = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++) {
            Ab[i] = (double *)malloc(N * sizeof(double));
        }
        double **An = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++) {
            An[i] = (double *)malloc(M * sizeof(double));
        }

        // initialize Ab inverse
        double **Ab_inverse = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++) {
            Ab_inverse[i] = (double *)malloc(N * sizeof(double));
        }

        find_AJ(A, basic_index, Ab);
        find_An(A, non_basic_index, An);
        
        // calculate Ab_inverse
        if (inverseMatrix(Ab, Ab_inverse, N)) {
            printf("%d", iter_num);
            printf("\n");
        } else {
            printf("Matrix is singular.\n");
            break;
        }
        

        // calculate p = (Ab_inverse)*cB
        double cB[N];
        for (int i = 0; i < N; i++) {
            cB[i] = c[basic_index[i]];
        }

        double p[N] = {0};
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                p[i] += cB[j]*Ab_inverse[j][i];
            }
        }

        // find cN_bar (reduced cost)
        double cN[M], cN_bar[M] = {0};
        for (int i = 0; i < M; i++) {
            cN[i] = c[non_basic_index[i]];
        }



        // p^t*A
        double pA[M] = {0};
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < num_row; j++) {
                 pA[i] += p[j]*An[j][i];
            }
        }

        for (int i = 0; i < M; i++) {
            cN_bar[i] = cN[i] - pA[i];
        }


        // check if find CN_bar < 0 and find entering variable
        int status = 0;
        int entering_index = 0;
        int small = 0;
        for (int i = 0; i < M; i++) {
            if (cN_bar[i] < small) {
                small = cN_bar[i];
                entering_index = i;
            } else{
                status += 1;
            }
        }

        int entering_var = non_basic_index[entering_index];

        double Xb[N];
            for (int i = 0; i < N; i++) {
                Xb[i] = 0;
                for (int j = 0; j < N; j++) {
                    Xb[i] += Ab_inverse[i][j] * b[j];
                }
            }

        // if find optimal
        if (status == M) {
            //printf("iter_num\n");
            //printf("%d", iter_num);
            //printf("\n");

            printf("basic variables\n");
            for (int j = 0; j < N; j++) {
                printf("%d%s", basic_index[j], j == N - 1 ? "" : " ");
            }
            printf("\n");

            printf("Xb\n");
            for (int j = 0; j < N; j++) {
                printf("%.3f%s", Xb[j], j == N - 1 ? "" : " ");
            }
            printf("\n");

            double z = 0;
            for (int i = 0; i < N; i++){
                z += p[i]*b[i];
            }
            printf("optimal value\n");
            printf("%f", z);

            break; // find optimal! end loop
        }

        double colA[num_row];
            for (int i = 0; i < num_row; i++) {
                colA[i] = A[i][entering_var];
            }
            
        double A_bar[num_row];
            for (int i = 0; i < num_row; i++) {
                A_bar[i] = 0;
                for (int j = 0; j < num_row; j++) {
                    A_bar[i] += Ab_inverse[i][j] * colA[j];
                }
            }

        // decide leaving variable (if tie, use Bland's rule)
        double min_ratio[num_row] = {0};
        int unboundedness = 0;
        
        for (int i = 0; i < num_row; i++) {
            if ( A_bar[i] > 0){
                min_ratio[i] = Xb[i]/ A_bar[i];
            } else {
                min_ratio[i] = -1.0;
                unboundedness += 1;
            }
        }

        if (unboundedness == num_row) {
            break; // LP is unbounded
        }

        int leaving_index = 0;
        leaving_index = findMin(min_ratio, num_row);
        int leaving_var = basic_index[leaving_index];
        
        // new basic index
        assign_variable(basic_index, non_basic_index, entering_var, leaving_var);

        // clean up memory
        for (int i = 0; i < M; i++) {
            free(Ab[i]);
            free(Ab_inverse[i]);
        }
        free(Ab);
        free(Ab_inverse);
    }

    return 0;
}
