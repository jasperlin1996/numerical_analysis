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
    int i = 0;

    if(x - 3 <= epsilon) xn -= 0.01;
    if(y - 4 <= epsilon) yn -= 0.01;


    fstream file;
    file.open("result.txt", ios::out);
    file << x << ' ' << y << '\n';

    printf("       |x_n                     y_n                     l2_norm     \n");
    printf("-------|------------------------------------------------------------\n");
    printf("%7d|%.16lf\t%.16lf\t---\n", i++, xn, yn);
    while(l2_norm > epsilon){ // not converged
    // for(int i = 0; i <= 20;){
        pfpx = dfdx(xn, yn);
        pfpy = dfdy(xn, yn);
        pgpx = dgdx(xn, yn);
        pgpy = dgdy(xn, yn);
        // cout << pfpx << " " << pfpy << " " << pgpx << " " << pgpy << endl;
        // solving the linear system
        h = (pgpy*f_value - pfpy*g_value)/(pfpy*pgpx - pfpx*pgpy);
        k = (-pgpx*f_value + pfpx*g_value)/(pfpy*pgpx - pfpx*pgpy);

        // update [xn, yn]
        xn = xn + h;
        yn = yn + k;
        l2_norm = sqrt(fabs(h*h) + fabs(k*k));

        f_value = f(xn, yn);
        g_value = g(xn, yn);
        printf("%7d|%.16lf\t%.16lf\t%.16lf\n", i, xn, yn, l2_norm);
        ++i;
        
        file << xn << " " << yn << '\n';    
    }
    file.close();
    double *solution = new double(2);
    solution[0] = xn;
    solution[1] = yn;

    return solution;
}

int main(){
    double init_x = 3.01, init_y = 3.99;
    cin >> init_x >> init_y;

    double *solution = newton_2d(init_x, init_y);
    cout << solution[0] << "\t" << solution[1] << endl;
}
