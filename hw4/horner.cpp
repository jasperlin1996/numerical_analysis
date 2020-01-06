#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

vector<double> find_c(vector<double> &x_data, vector<double> &y_data){
    // f_data[i] == {t, x, y};
    int n = x_data.size();
    vector<double> dp_table;
    dp_table.assign(y_data.begin(), y_data.end());
    
    for(int i = 1; i < n; i++){
        for(int j = n-1; j >= i; j--){
            dp_table[j] = (dp_table[j] - dp_table[j-1])/(x_data[j]-x_data[j-i]);
            // cout << dp_table[j] << ' ';
        }
        // cout << endl;
    }
    return dp_table;
}

double horner_p(vector<double> &x, vector<double> &x_coef, double t){
    double sum = x_coef.back();
    for(int i = x_coef.size() - 2; i >= 0; i--){
        sum = sum*(t - x[i]) + x_coef[i];
    }
    // cout << t << ' ' << sum << endl;
    return sum;
}

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

void chord(vector<double> &t, vector<double> x, vector<double> y){
    double temp = 0, length = 0;
    for(int i = 0; i < x.size() - 1; i++){
        length += distance(x[i], y[i], x[i+1], y[i+1]);
    }
    for(int i = 1; i < t.size(); i++){
        temp += distance(x[i-1], y[i-1], x[i], y[i]);
        t[i] = temp/length;
    }
}

void interpolation(vector<double> t, vector<double> x, vector<double> y, string mode, string file_name){
    vector<double> x_coef, y_coef;
    x_coef = find_c(t, x);
    y_coef = find_c(t, y);
    for(int i = 0; i < x_coef.size(); i++){
        cout << "cx" << i << ": " << x_coef[i] << '\n';
        cout << "cy" << i << ": " << y_coef[i] << '\n';
    }
    cout << endl;
    
    fstream file;
    file << fixed << setprecision(10);
    file.open(mode + '_' + file_name, ios::out);
    cout << "====================================\n";
    cout << "\t\t" << mode << endl;
    cout << "====================================\n";
    double range = t[t.size() - 1], inter_x, inter_y;
    for(double i = 0.0; i <= range+0.00001; i += range/100.0){
        cout << "t: " << i << endl;
        inter_x = horner_p(t, x_coef, i);
        inter_y = horner_p(t, y_coef, i);
        cout << inter_x << ", " << inter_y << '\n';
        file << inter_x << ' ' << inter_y << '\n'; 
    }
    file.close();
}

int main(int argc, char** argv){
    vector<double> t, x, y;
    double pt, px, py;
    fstream file;
    string file_name= argv[1];

    file.open(file_name, ios::in);
    while(file >> pt >> px >> py){
        t.push_back(pt);
        x.push_back(px);
        y.push_back(py);
    }
    file.close();

    interpolation(t, x, y, "uniform", file_name);
    chord(t, x, y);
    for(int i = 0; i < t.size(); i++){
        cout << "t" << i << ": " << t[i] << endl;
    }
    interpolation(t, x, y, "chord", file_name);
}
