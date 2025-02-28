#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <bits/stdc++.h>
// using namespace std;
// LP problem data                 standard form
// min z = 2x1 + 3x2               min Z = 2x1 + 3x2 
// s.t. x1 + 2x2 <= 4              s.t. x1 + 2x2 + s1 = 4
//      x1 + x2  <= 3                   x1 + x2 + s2 = 3
//      x1, x2 >= 0                   

// data is LP problem in standard form
// min z = cx
// s.t. Ax = b
//      x >= 0
#define N 2
#define M 2
#define num_row 2
#define num_col 4

double A[2][4] = {
    {1, 2, 1, 0},
    {1, 1, 0, 1}
};
double b[2] = {4, 3};
double c[4] = {2, 3, 0, 0};


// calculate cofactor
void cofactor(double Ab[N][N], double cof[N][N], int s, int t, int n){
    int i = 0, j = 0;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++){
            if (row != s && col != t) {
                cof[i][j++] = Ab[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// calaulate determinant
double Determinant(double Ab[N][N], int n) {
    if (n == 1) return Ab[0][0];

    double det = 0.0;
    double cof[N][N];
    int sign = 1;

    for (int f = 0; f < n; f++) {
        cofactor(Ab, cof, 0, f, n);
        det += sign * Ab[0][f] * Determinant(cof, n-1);
        sign = -sign;
    }
    return det;
}


// calculate Ab-1
int inverse(double Ab[N][N], double Ab_inverse[N][N]){
    double det = Determinant(Ab, N);
    
    if (det == 0) {
        printf("Singular\n");
        return -1;
    }

    double adj[N][N];
    int sign = 1;
    double cof[N][N];

    // calculate the adjugate
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cofactor(Ab, cof, i, j, N);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = sign * Determinant(cof, N-1);
        }
    }
    
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Ab_inverse[i][j] = adj[i][j] / det;
        }
    }

    return 0;
}

// Use basic index to find Ab, An
void find_AJ(double A[num_row][num_col], int basis[], double Aj[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Aj[i][j] = A[i][basis[j]];
        }
    }
}

int findMin(double arr[], int n) {
    int min = arr[0];
    int min_index = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] < 0){
            continue;
        }

        if (arr[i] < min) {
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

    // int num_row = sizeof(A) / sizeof(A[0]);
    // int num_col = sizeof(A[0]) / sizeof(A[0][0]);

    //int N = 2; // number of basic variable
    //int M = num_col - N; // number of non basic variable

    int status = 0;
    // initial basis varible index
    int basic_index[N] = {2, 3};
    int non_basic_index[M] = {0, 1};

    int iter_num = 0;
    while (iter_num <= 100) {
        iter_num++;
        double Ab[N][N];
        find_AJ(A, basic_index, Ab);

        double An[N][M];
        find_AJ(A, non_basic_index, An);

        // calculate Ab_inverse
        double Ab_inverse[N][N];
        inverse(Ab, Ab_inverse);

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
        int entering_index = 0;
        for (int i = 0; i < M; i++) {
            if (cN_bar[i] < 0) {
                entering_index = i; // (use Bland's rule)
                break;
            }
            status = 1;
        }

        int entering_var = non_basic_index[entering_index];

        // if find optimal
        if (status == 1) {

            double Xb[N];
            for (int i = 0; i < N; i++) {
                Xb[i] = 0;
                for (int j = 0; j < N; j++) {
                    Xb[i] += Ab_inverse[i][j] * b[j];
                }
            }

            for (int j = 0; j < N; j++) {
                printf("%.3f%s", Xb[j], j == N - 1 ? "" : " ");
            }
            printf("\n");

            double z = 0;
            for (int i = 0; i < N; i++){
                z += p[i]*b[i];
            }
            z = -1*z;
            printf("%f", z);

            break; // find optimal! end loop
        }

        // decide leaving variable (if tie, use Bland's rule)
        double min_ratio[num_row] = {0};
        int unboundedness = 0;
        
        for (int i = 0; i < num_row; i++) {
            if (A[i][entering_index] > 0){
                min_ratio[i] = b[i]/A[i][entering_index];
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

        assign_variable(basic_index, non_basic_index, entering_var, leaving_var);

        // new basis Ab
        printf("\n");
    }


    return 0;
}