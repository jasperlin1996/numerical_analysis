#include "Matrix.h"
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

int main(){
    vector<vector<double> > vec{{1}, {2}, {3}};
    vector<vector<double> > vec2{{2}, {3}, {4}};
    Matrix a(vec);
    Matrix b(vec2);
    b = b.T() * a;
    cout << b;
}