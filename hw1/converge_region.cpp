/* Author: Jasper Lin
 * 
 * Root finding using Newton's method
 * target function:
 *      f(x, y) = ((x - 3)**2)/25 + ((y - 4)**2)/16 - 1 = 0
 *      g(x, y) = y - x**2 - 2x - 1 = 0
 * Requirement:
 *      (a) Init [3.01, 3.99] and compute [x y]^(i), i = 1, 2, 3...
 *      (b) List all [x_i, y_i] and {f(x_i, y_i), g(x_i, y_i)} 
 *          for 0 <= i <= 20
 *      (c) If the computation converges, how many iterations are
 *          required in order to achieve |e_n - e_{n-1}| <= 10^-6?
 *          Please use 2-norn to estimate the magnitude of the 
 *          error vector.
 *      (d) Try at least 4 other initial solutions (one initial 
 *          solution in one quadrant, the center = [3, 4]) and 
 *          compute the roots.
 *
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

double f(double x, double y){
    return ((x - 3)*(x - 3)) / 25 + ((y - 4)*(y - 4)) / 16 - 1;
}
double g(double x, double y){
    return y - x*x - 2*x - 1;
}
double dfdx(double x, double y){
    return (x*2-6)/25;
}
double dfdy(double x, double y){
    return (y-4)/8;
}
double dgdx(double x, double y){
    return - 2*x - 2;
}
double dgdy(double x, double y){
    return 1; 
}
double* newton_2d(double x, double y){
    // random num
    random_device rd;
    default_random_engine gen = default_random_engine(rd());
    uniform_real_distribution<> dis(-1, 1);
    double random = dis(gen);
    const double epsilon = 0.000001;
    double xn = x, yn = y;
    double f_value = f(x, y), g_value = g(x, y);
    double pfpx, pfpy, pgpx, pgpy, h, k, l2_norm = INT_MAX;
    double *solution = new double(2);
    solution[0] = INT_MAX;
    solution[1] = INT_MAX;

    if(x - 3 <= epsilon) xn -= 0.01;
    if(y - 4 <= epsilon) yn -= 0.01;
    int i = 0;
    while(l2_norm > epsilon){ // not converged
        if(i >= 200){
            return solution;
        }
        pfpx = dfdx(xn, yn);
        pfpy = dfdy(xn, yn);
        pgpx = dgdx(xn, yn);
        pgpy = dgdy(xn, yn);
        double det = pfpy*pgpx - pfpx*pgpy;
        if(fabs(det) <= epsilon){
            xn -= random;
            yn += random;
            continue;
        }
        // solving the linear system
        h = (pgpy*f_value - pfpy*g_value)/(pfpy*pgpx - pfpx*pgpy);
        k = (-pgpx*f_value + pfpx*g_value)/(pfpy*pgpx - pfpx*pgpy);

        // update [xn, yn]
        xn = xn + h;
        yn = yn + k;
        l2_norm = sqrt(fabs(h*h) + fabs(k*k));

        f_value = f(xn, yn);
        g_value = g(xn, yn);
        ++i;
    }
    solution[0] = xn;
    solution[1] = yn;

    return solution;
}
double determine(double xn, double yn){
    double pfpx, pfpy, pgpx, pgpy, epsilon = 0.000001;
    pfpx = dfdx(xn, yn);
    pfpy = dfdy(xn, yn);
    pgpx = dgdx(xn, yn);
    pgpy = dgdy(xn, yn);

    if(fabs(pfpy*pgpx - pfpx*pgpy) <= epsilon){
        return 0;
    }
    return (pfpy*pgpx - pfpx*pgpy);

}
int main(){
    double *solution;
    int counter = 0;
    double step = 0.1;
    fstream file_nope, file_green, file_red, file_determine;
    file_nope.open("converge_nope.txt", ios::out);
    file_green.open("converge_green.txt", ios::out);
    file_red.open("converge_red.txt", ios::out);
    file_determine.open("determine.csv", ios::out);

    for(double i = -150.0; i <= 150.0; i += step){
        stringstream ss;
        for(double j = -150.0; j <= 150.0; j += step, counter += 1){
            string str = "";
            for(int k = 0; k < 50; k++){
                if(counter/(180000*1) > k) str += "*";
                else str += " ";
            }
            ss << determine(j, i) << ",";

            cout << flush << ("\r|" + str + "|\t" + to_string(counter/(90000.0*1)) + "%");
            solution = newton_2d(i, j);

            if(solution[0] == INT_MAX && solution[0] == INT_MAX){
                file_nope << i << ' ' << j << '\n';
            }
            if(fabs(solution[0] - 1.8079638) <= 0.000001 && fabs(solution[1] - 7.8846611) <= 0.000001){
                file_green << i << ' ' << j << '\n';
            }
            if(fabs(solution[0] + 0.0789396) <= 0.000001 && fabs(solution[1] - 0.848352) <= 0.000001){
                file_red << i << ' ' << j << '\n';
            }
        }
        string tmp;
        ss >> tmp;
        tmp[tmp.length() - 1] = '\n';
        file_determine << tmp;
    }
    file_nope.close();
    file_green.close();
    file_red.close();
    file_determine.close();
}
