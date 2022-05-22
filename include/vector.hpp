#pragma once
#include <iostream>
#include <cmath>
#include <vector>


class Vector
{
public:
    explicit Vector(double, double, double);
    Vector();
    double norm2() const;
    double norm() const;
    void normalize();
    double operator[](int i) const { return data[i]; };
    double &operator[](int i) { return data[i]; };
    double data[3];
};

void print_vector(const Vector &a);
Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
Vector operator*(const double a, const Vector &b);
Vector operator*(const Vector &a, const double b);
Vector operator/(const Vector &a, const double b);
double dot(const Vector &a, const Vector &b);
Vector cross(const Vector &a, const Vector &b);
bool operator==(const Vector &a, const Vector &b);
bool operator!=(const Vector &a, const Vector &b);
std::ostream &operator<<(std::ostream &os, const Vector &v);
double distance(const Vector &a, const Vector &b);
