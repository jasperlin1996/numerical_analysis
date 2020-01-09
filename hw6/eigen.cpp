/* Hint for myself:
    The point of max heap version is at function: update_A_mtx().
    The function will update Matrix A 

        A[p][k] = A[k][p] = Bp[k];
        A[q][k] = A[k][q] = Bq[k];

    in a for-loop execute k times, which means that if we maintain
    the Matrix A here, we will be able to peek the max entry in
    O(2logN) + O(1) = O(2logN) as the heap provides to us.
*/

#define _USE_MATH_DEFINES
#define _EPSILON 1e-6
#include "Matrix.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <functional>

using namespace std;

Matrix gen_sym_mtx(vector<vector<double>> &A/*double **A*/, int n){
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<double> dis(0.0, 20.0);
    auto randfun = bind(dis, gen);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            A[i][j] = A[j][i] = randfun();
        }
        A[i][i] += 3.0;
    }
    Matrix m(A);
    cout << m;
    return m;
}

Matrix gen_sym_mtx(int n){
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<double> dis(0.0, 20.0);
    auto randfun = bind(dis, gen);
    Matrix A(n, n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            A[i][j] = A[j][i] = randfun();
        }
        A[i][i] += 3.0;
    }
    cout << A;
    return A;
}

vector<double> flatten(Matrix A){
    int rows = A.getShape().first;
    int cols = A.getShape().second;
    vector<double> flattened(rows*cols, 0.0);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            flattened[i*rows + j] = A[i][j];
        }
    }
    return flattened;
}

double max_off_diag_entry(Matrix &A, int &p, int &q){ // No heap version
    // Suppose A is a symmetric matrix
    int rows = A.getShape().first, cols = A.getShape().second;
    double max_value = -__DBL_MAX__;
    
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i != j && fabs(A[i][j]) > max_value) {
                max_value = fabs(A[i][j]);
                p = i;
                q = j;
            }
        }
    }
    return max_value;
}

void update_P_mtx(Matrix &P, int p, int q, double c, double s){
    int rows = P.getShape().first;
    vector<double> Qp(rows, 0.0);
    vector<double> Qq(rows, 0.0);
    for(int k = 0; k < rows; k++){ // col operations, range in rows.
        Qp[k] =  c*P[k][p] + s*P[k][q];
        Qq[k] = -s*P[k][p] + c*P[k][q];
    }
    for(int k = 0; k < rows; k++){
        P[k][p] = Qp[k];
        P[k][q] = Qq[k];
    }
}
void update_A_mtx(Matrix &A, int p, int q, double c, double s){
    int n = A.getShape().first;
    vector<double> Bp(n, 0.0);
    vector<double> Bq(n, 0.0);
    Bp[p] = c*c*A[p][p] + 2.0*s*c*A[p][q] + s*s*A[q][q];
    Bq[q] = s*s*A[p][p] - 2.0*s*c*A[p][q] + c*c*A[q][q];
    for(int r = 0; r < n; r++){
        if(r != p && r != q) {
            Bp[r] =  c*A[r][p] + s*A[r][q];
            Bq[r] = -s*A[r][p] + c*A[r][q];
        }
    }
    for(int k = 0; k < n; k++){ 
        A[p][k] = A[k][p] = Bp[k];
        A[q][k] = A[k][q] = Bq[k];
    }
    A[p][q] = A[q][p] = 0.0;
}

pair<Matrix, Matrix> jacobian(Matrix m, ostream &os){

    fstream file("offDiagVar.txt", ios::out);

    Matrix A(m);
    int rows = A.getShape().first, cols = A.getShape().first, p, q, iterations = 0;
    Matrix P = Matrix::identity(rows, cols);
    double Apq = max_off_diag_entry(A, p, q);

    while(fabs(Apq) > _EPSILON){
        iterations++;
        double theta = atan((A[p][q] * 2.0)/(A[p][p] - A[q][q]))/2.0;
        double c = cos(theta);
        double s = sin(theta);
        update_P_mtx(P, p, q, c, s); // P = P*R;
        update_A_mtx(A, p, q, c, s); // B = R.T()*A*R;
        Apq = max_off_diag_entry(A, p, q);
        double offDiag = 0.0;
        // Store offDiag[k]
        for(int i = 0; i < rows; i++){
            for(int j = i + 1; j < cols; j++){
                offDiag += fabs(A[i][j]);
            }
        }
        file << iterations << ' ' << offDiag << endl;
    }
    file.close();
    cout << A;
    // N = 3 ~ 25
    os << rows << ' ' << iterations << endl;
    return make_pair(A, P); // pair<eigenvalue, eigenvector>
}

double inf_norm(Matrix &A){
    int rows = A.getShape().first, cols = A.getShape().second;
    double norm = 0.0;
    for(int i = 0; i < rows; i++){
        double tmp = 0.0;
        for(int j = 0; j < cols; j++){
            tmp += fabs(A[i][j]);
        }
        if(tmp >= norm) norm = tmp;
    }
    return norm;
}

void experiment(ostream &os){
    Matrix A;
    for(int N = 3; N <= 25; N++){
        A = gen_sym_mtx(N);
        pair<Matrix, Matrix> eigen = jacobian(A, os);
        // os << "Eigen Values: " << eigen.first;
        // os << "Eigen Vectors: " << eigen.second;
        // os << "V * V.T: " << eigen.second*eigen.second.T();
    }
}

int main(){
    Matrix A = gen_sym_mtx(4);
    cout << "After jacobian procedure: ";
    pair<Matrix, Matrix> eigen = jacobian(A, cout);
    cout << "Eigen Values: " << eigen.first;
    cout << "Eigen Vectors: " << eigen.second;

    Matrix tmp = A*eigen.second - eigen.first*eigen.second;
    cout << "INF Norm: " << inf_norm(tmp) << endl;
    cout << tmp;

    cout << "V * V.T: " << eigen.second*eigen.second.T();
    // fstream file("result.txt", ios::out);
    // experiment(file);
    // file.close();
}
