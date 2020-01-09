#include "Matrix.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#define PRECISION 8
using namespace std;

Matrix::Matrix(){
    this->data = vector<vector<double> >();
    this->shape = pair<int, int>(0, 0);
}

Matrix::Matrix(const Matrix &rhs){
    this->data = rhs.data;
    this->shape = rhs.shape;
}

Matrix::Matrix(int row, int col){
    this->data = vector<vector<double> >(row, vector<double>(col, 0.0));
    this->shape = make_pair(row, col);
}

Matrix::Matrix(vector<vector<double> > m){
    this->data = m;
    this->shape = pair<int, int>(this->data.size(), this->data[0].size());
}

Matrix::~Matrix(){

}

pair<int, int> Matrix::getShape(){
    return this->shape;
}

ostream& operator<<(ostream &os, Matrix const &m){
    double max = 0.0;
    int space = 0;
    os << m.shape.first << ", " << m.shape.second << endl;
    os << fixed << setprecision(PRECISION);
    for(auto i = 0 ; i < m.data.size(); i++){
        os << ' ';
        for(auto j = 0; j < m.data[i].size(); j++){
            os << m.data[i][j] << (j == (m.data[i].size() - 1) ? '\n' : '\t');
        }
    }
    return os;
}

vector<double>& Matrix::operator[](int index){
    try{
        return this->data.at(index);
    }
    catch(const std::out_of_range& e){
        cout << "Shape: (" << this->shape.first << ", " << this->shape.second << ")\n";
        cout << "Out of range at index " << index << " bro : (\n"; 
        throw e;
    }
}

struct MatrixShapeNotEqual : public std::exception
{

};

Matrix Matrix::operator+(const Matrix &rhs){
    Matrix tmp = *this;
    if(tmp.shape == rhs.shape){
        for(auto i = 0; i < tmp.data.size(); i++){
            for(auto j = 0; j < tmp.data[i].size(); j++){
                tmp.data[i][j] += rhs.data[i][j];
            }
        }
        return tmp;
    }
    else{
        throw MatrixShapeNotEqual();
    }
}

Matrix Matrix::operator+=(const Matrix &rhs){
    *this = *this+rhs;
    return *this;
}

Matrix Matrix::operator-(const Matrix &rhs){
    Matrix tmp = *this;
    if(tmp.shape == rhs.shape){
        for(auto i = 0; i < tmp.data.size(); i++){
            for(auto j = 0; j < tmp.data[i].size(); j++){
                tmp.data[i][j] -= rhs.data[i][j];
            }
        }
        return tmp;
    }
    else{
        throw MatrixShapeNotEqual();
    }
}

Matrix Matrix::operator-=(const Matrix &rhs){
    *this = *this-rhs;
    return *this;
}

Matrix Matrix::T(){
    vector<vector<double> > tmp(this->shape.second, vector<double>(this->shape.first));
    for(auto i = 0; i < tmp.size(); i++){
        for(auto j = 0; j < tmp[i].size(); j++){
            tmp[i][j] = this->data[j][i];
        }
    }
    return Matrix(tmp);
}

struct MatrixShapeNotMatch : public std::exception
{

};

Matrix Matrix::operator*(const Matrix &rhs){
    if(this->shape.second == rhs.shape.first){
        vector<vector<double> > tmp(this->shape.first, vector<double>(rhs.shape.second, 0.0));
        for(auto i = 0; i < tmp.size(); i++){
            for(auto j = 0; j < tmp[i].size(); j++){
                for(auto k = 0; k < this->shape.second; k++){
                    tmp[i][j] += this->data[i][k]*rhs.data[k][j];
                }
            }
        }
        return Matrix(tmp);
    }
    else{
        throw MatrixShapeNotMatch();
    }
}

Matrix Matrix::operator*=(const Matrix &rhs){
    *this = *this*rhs;
    return *this;
}

Matrix Matrix::matmul(const Matrix &rhs){
    return *this*rhs;
}

Matrix Matrix::dot(const Matrix &rhs){
    if(this->shape == rhs.shape){
        vector<vector<double> > tmp(this->shape.first, vector<double>(this->shape.second));
        for(auto i = 0; i < tmp.size(); i++){
            for(auto j = 0; j < tmp[i].size(); j++){
                tmp[i][j] = this->data[i][j] * rhs.data[i][j];
            }
        }
        return Matrix(tmp);
    }
    else{
        throw MatrixShapeNotEqual();
    }
}

// Return a identity Matrix
// Usage: Matrix::identity(row, col);
Matrix Matrix::identity(int row, int col){
    vector<vector<double> > tmp(row, vector<double>(col, 0.0));
    for(auto i = 0; i < min(row, col); i++){
        tmp[i][i] = 1;
    }
    return Matrix(tmp);
}

Matrix Matrix::operator*(const double lambda){
    Matrix tmp(*this);
    for(auto i = 0; i < tmp.shape.first; i++){
        for(auto j = 0; j < tmp.shape.second; j++){
            tmp[i][j] = tmp[i][j]*lambda;
        }
    }
    return tmp;
}

Matrix operator*(const double lambda, Matrix &rhs){
    return rhs*lambda;
}