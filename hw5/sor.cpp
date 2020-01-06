#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#define H 21
#define EDGE_L 30
#define EDGE_R 30
#define EDGE_B 20

using namespace std;

double matrix_inf_norm(vector<vector<double>> &T){
    double max = 0.0;
    for(auto i = 1; i < T.size() - 1; i++){
        double tmp = 0.0;
        for(auto j = 1; j < T.size() - 1; j++){
            tmp += fabs(T[i][j]);
        }
        if(tmp > max) max = tmp;
    }
    cout << max << endl;
    return max;
}

int SOR(vector<vector<double>> &T, fstream& file, double omega = 1.0){
    // sourse temperature
    vector<double> s(H*H, 0.1);
    s[(H*H)/2] = -5.0; // Heat sourse, i'm not sure

    double Tn_norm = 10.0, Tn1_norm = matrix_inf_norm(T);
    int iter = 0;
    stringstream ss;

    while(fabs(Tn_norm - Tn1_norm) > 0.00001 ){
        ss << "Iteration = " << iter << endl;
        cout << "Iteration = " << iter++ << endl;

        for(int i = 0; i < H - 1; i++){ // 19*19
            for(int j = 1; j < H - 1; j++){
                double ek = 0.0;
                if (i != 0) ek = s[i*H + j]*(1.0/(H*H)) - (-4.0*T[i][j] + T[i-1][j] + T[i+1][j] + T[i][j-1] + T[i][j+1]); // /(double)((H-1)*(H-1));
                else ek = s[i*H + j] - (-4.0*T[i][j] + T[i+1][j] + T[i][j-1] + T[i][j+1]); // /(double)((H-1)*H);
                // cout << ek << endl;
                T[i][j] = T[i][j] + (omega/-4)*ek;
            }
        }
        for(int i = H-1; i >= 0; i--){
            for(int j = 0; j < H; j++){
                ss << T[i][j] << ' ';
            }
            ss << endl;
        }
        ss << endl;
        Tn_norm = Tn1_norm;
        Tn1_norm = matrix_inf_norm(T);
    }
    file << "Size = " << H << " x " << H << " , Iteration = " << iter - 1 << endl;
    file << ss.str() << flush;
    return iter - 1;
}

int experiment(string filename, double omega = 1.0){
    vector<vector< double>> temp(H, vector<double>(H, 10.0)); // temperature
    // init
    fstream file(filename, ios::out);
    for(int i = 0; i < H; i++){
        temp[i][0] = EDGE_L;
        temp[i][temp[i].size() - 1] = EDGE_R;
        temp[temp.size() - 1][i] = EDGE_B;
    }
    int iter = SOR(temp, file, omega);
    for(int i = 0; i < H; i++){
        for(int j = 0; j < H; j++){
            cout << temp[i][j] << '\t';
        }
        cout << endl;
    }
    file.close();
    return iter;
}

int main(){
    // int size = 11;
    // double exps_omega[size] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.85, 1.9};
    // string file_omega[size] = {"1_0", "1_1", "1_2", "1_3", "1_4", "1_5", "1_6", "1_7", "1_8", "1_85", "1_9"};
    // fstream file("omega_compare.txt", ios::out);
    // for(int i = 0; i < size; i++){
    //     int iter = experiment("result_" + file_omega[i] + ".txt", exps_omega[i] );
    //     file << exps_omega[i] << " " << iter << endl;
    // }
    // file.close();

    // Exp 1
    experiment("result_GS.txt", 1.0);
    experiment("result_SOR.txt", 1.2);
}