// #pragma once
#include <iostream>
#include <vector>
using namespace std;

class Matrix {
public:
    // basic structure
    Matrix();
    Matrix(const Matrix &);
    Matrix(int, int);
    Matrix(vector<vector<double> >);
    ~Matrix();
    friend ostream& operator<<(ostream& os, const Matrix& m);
    vector<vector<double> > data;
    pair<int, int> shape;

    //
    vector<double>& operator[](int);
    Matrix operator+(const Matrix&);
    Matrix operator+=(const Matrix&);
    Matrix operator-(const Matrix&);
    Matrix operator-=(const Matrix&);
    pair<int, int> getShape();

    //
    Matrix T();
    Matrix operator*(const Matrix&);
    Matrix operator*(const double);
    Matrix operator*=(const Matrix&);
    Matrix matmul(const Matrix&);
    Matrix dot(const Matrix&);

    //
    static Matrix identity(int, int);
private:

};

Matrix operator*(const double lambda, Matrix &rhs);