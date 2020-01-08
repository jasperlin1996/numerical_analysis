#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include "Matrix.h"
using namespace std;

void gen_sym_mtx(vector<vector<double> > A/*double **A*/, int n){
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

}

int main(){
    // double **A;
    int n = 3;
    vector<vector<double> > A(3, vector<double>(3, 0.0));
    gen_sym_mtx(A, n);
}
