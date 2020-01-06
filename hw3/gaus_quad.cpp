#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

// N start from 0
vector<vector<double> > w{
    {2},
    {1, 1},
    {0.88888888888888888889, 0.55555555555555555556, 0.55555555555555555556},
    {0.65214515486254614262, 0.65214515486254614262, 0.34785484513745385737, 0.34785484513745385737},
    {0.56888888888888888889, 0.47862867049936646804, 0.47862867049936646804, 0.23692688505618908751, 0.23692688505618908751}
};
vector<vector<double> > pos{
    {0},
    {-0.57735026918962576450, 0.57735026918962576450},
    {0, -0.77459666924148337703, 0.77459666924148337703},
    {-0.33998104358485626480, 0.33998104358485626480, -0.86113631159405257522, 0.86113631159405257522},
    {0, -0.53846931010568309103, 0.53846931010568309103, -0.90617984593866399279, 0.90617984593866399279}
};

double f(double x, double y){
    double ret = 1.0;
    if(fabs(x) >= 1e-7) ret *= (sin(M_PI*x)/(M_PI*x));
    if(fabs(y) >= 1e-7) ret *= (sin(M_PI*y)/(M_PI*y));
    return ret;
    // return x*x + y*y + 1;
}

double mapping(double x, double a, double b){
    return (b-a)*x/2.0 + (a+b)/2.0;
}

double gaus_quad(double n_sample, double a1, double b1, double a2, double b2){
    double sum = 0;
    for(unsigned int i = 0; i < n_sample; i++){
        for(unsigned int j = 0; j < n_sample; j++){
            sum += w[n_sample-1][i]
                * w[n_sample-1][j]
                * f(mapping(pos[n_sample-1][i], a1, b1), mapping(pos[n_sample-1][j], a2, b2));
        }
    }
    return sum*(b1-a1)*(b2-a1)/4.0;
}

int main(){
    cout << fixed << setprecision(10);
    double exact_ans = 1.38999323787; // by wolframalpha
    for(int n_sample = 2; n_sample <=4; n_sample++){
        for(int split = 1; split <= 3; split++){
            double sum = 0;
            for(double x = -1, x_times = 0; x_times < split; x_times++){
                for(double y = -1, y_times = 0; y_times < split; y_times++){
                    sum += gaus_quad(n_sample, x, x + 2.0/split, y, y + 2.0/split);
                    y += (2.0/split);
                }
                x += (2.0/split);
            }

            cout << "n_sample, split = (" << n_sample << ", " << split*split << "), " << sum;
            cout << ", error = " << fabs(exact_ans - sum) << endl;
        }
    }
}
