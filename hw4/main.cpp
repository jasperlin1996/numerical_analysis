#include "Matrix.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

Matrix gaussian_eliminate(Matrix m, Matrix b){
    Matrix original(m);
    int rows = m.getShape().first;
    int cols = m.getShape().second;
    for(int i = 0; i < cols; i++){
        double max = fabs(m[i][i]), max_pivot = i;
        for(int j = i + 1; j < rows; j++){
            if(m[j][i] > max){
                max = fabs(m[j][i]);
                max_pivot = j;
            }
        }
        // exchange
        vector<double> tmp = m[i];
        m[i] = m[max_pivot];
        m[max_pivot] = tmp;
        tmp = b[i];
        b[i] = b[max_pivot];
        b[max_pivot] = tmp;

        // elimination
        for(int j = i + 1; j < rows; j++){
            if(m[j][i] == 0.0) continue;
            double alpha = -(m[j][i])/(m[i][i]);
            for(int k = i; k < cols; k++){ // TODO
                m[j][k] = m[j][k] + alpha*m[i][k];
            }
            b[j][0] = b[j][0] + alpha*b[i][0];
        }
    }
    // cout << m;
    
    Matrix solution(cols, 1);
    double left_side;
    // backward substitution
    // TODO: may have some problems with non-square matrix
    for(int i = cols - 1; i >= 0; i--){
        left_side = 0.0;
        if(m[i][i] != 0.0){
        for(int j = i + 1; j < cols; j++){
            left_side += solution[j][0]*m[i][j];
        }
            solution[i][0] = (b[i][0] - left_side)/m[i][i];
        }
    }
    // cout << original*solution;
    return solution;
}

Matrix create_v(Matrix m, int j){
    double x_norm = 0.0;
    Matrix ret(m.shape.first, 1);
    for(auto i = j; i < m.shape.first; i++){
        ret[i][0] = m[i][j];
        x_norm += m[i][j]*m[i][j];
    }
    x_norm = sqrt(x_norm);
    if(ret[j][0] >= 0) ret[j][0] += x_norm;
    else ret[j][0] -= x_norm;
    return ret;
}

Matrix QR(Matrix m, Matrix b){
    // forward elimination
    Matrix original(m);
    int rows = m.getShape().first;
    int cols = m.getShape().second;
    
    for(int j = 0; j < cols; j++){
        Matrix v = create_v(m, j); // vector v(rows, 1);
        Matrix vT = v.T();
        double vTv = (v.T()*v)[0][0];
        
        Matrix t(rows, 1);
        for(int k = j; k < cols; k++){
            for(int i = j; i < rows; i++){
                t[i][0] = m[i][k];
            }
            double vTt = (v.T()*t)[0][0];
            for(int i = j; i < rows; i++){
                m[i][k] = m[i][k] - 2.0*(vTt/vTv)*v[i][0];
            }
        }
        double vTb = (v.T()*b)[0][0];
        // for(int k = j; k < col; k++){
        //     b[k] = b[k] - 2.0*(vTb/vTv)*v[k];   
        // }
        
        b = b - 2.0*(vTb/vTv)*v;
    }

    Matrix solution(cols, 1);
    double left_side;
    // backward substitution
    for(int i = cols - 1; i >= 0; i--){
        left_side = 0.0;
        if(m[i][i] != 0.0){
            for(int j = i + 1; j < cols; j++){
                left_side += solution[j][0]*m[i][j];
            }
            solution[i][0] = (b[i][0] - left_side)/m[i][i];
        }
        else i++;
    }
    // cout << original*solution;
    return solution;
}

double horner(double x, int degree){
    double sum = 1.0;
    for(int i = 1; i < degree + 1; i++){
        double tmp = 1.0;
        for(int j = 0; j < i; j++){
            tmp *= x;
        }
        sum += tmp;
    }
    return sum;
}

void cal_error(Matrix solution, Matrix ans, fstream& os){
    double norm_2 = 0.0, norm_inf = 0.0;
    for(int i = 0; i < solution.shape.first; i++){
        double error = ans[i][0] - solution[i][0];
        norm_2 += error * error;
        if(fabs(error) > norm_inf) norm_inf = fabs(error);
    }
    norm_2 = sqrt(norm_2);
    cout << "\n---------- 2-norm ------------\n";
    cout << norm_2 << endl;
    cout << "\n--------- inf-norm -----------\n";
    cout << norm_inf << endl;
    cout << "------------------------------\n";
    os << setprecision(15) << norm_2 << " " << norm_inf << endl;
}

int main(){
    // high probability getting nan or -inf
    // vector<vector<double> > vec = {{1, 2, 2}, {1, 3, 5}, {2, 3, 4}, {2,2,2}, {1,1,1}};
    // vector<vector<double> > y = {{-1}, {-8}, {-4}, {0}, {0}};
    fstream f_ng, f_nqr, f_oqr;
    f_ng.open("./ng.txt", ios::out);
    f_nqr.open("./nqr.txt", ios::out);
    f_oqr.open("./oqr.txt", ios::out);
    for(int degree = 4; degree < 16; degree++){
        vector<vector<double> > vec(degree + 4, vector<double>(degree + 1, 0.0));
        vector<vector<double> > y(degree + 4, vector<double>(1, 0.0));
        vector<vector<double> > c(degree + 1, vector<double>(1, 1.0));
        
        for(int i = 0; i < degree + 4; i++){
            vec[i][0] = 1;
            for(int j = 1; j < degree + 1; j++){
                vec[i][j] = vec[i][j-1]*(2.0 + i*0.2);
            }
            y[i][0] = horner(2.0 + i*0.2, degree);
        }
        Matrix m(vec);
        Matrix b(y);
        Matrix ans(c);
        Matrix solution;
        // cout << m;
        // cout << b;

        // cout << m.T()*m;
        cout << "\n[NEW] gaussian elimination: \n";
        for(int i = 0; i < 10000; i++)
            solution = (gaussian_eliminate(m.T()*m, m.T()*b));
        cout << "\n--------- solution -----------\n";
        cout << solution;

        cal_error(solution, ans, f_ng);

        cout << "\n[NEW] QR decomposition: \n";
        for(int i = 0; i < 1; i++)
            solution = (QR(m.T()*m, m.T()*b));
        cout << "\n--------- solution -----------\n";
        cout << solution;

        cal_error(solution, ans, f_nqr);

        cout << "\n[ORIGINAL] QR decomposition: \n";
        for(int i = 0; i < 1; i++)
            solution = (QR(m, b));
        cout << "\n--------- solution -----------\n";
        cout << solution;

        cal_error(solution, ans, f_oqr);
    }
}