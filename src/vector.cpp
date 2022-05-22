#include "../include/vector.hpp"

Vector::Vector(double x = 0, double y = 0, double z = 0)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
}
double Vector::norm2() const
{
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
}
double Vector::norm() const
{
    return sqrt(norm2());
}
void Vector::normalize()
{
    double n = norm();
    data[0] /= n;
    data[1] /= n;
    data[2] /= n;
}

void print_vector(const Vector &a)
{
    std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl;
}

Vector operator+(const Vector &a, const Vector &b)
{
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
Vector operator-(const Vector &a, const Vector &b)
{
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
Vector operator*(const double a, const Vector &b)
{
    return Vector(a * b[0], a * b[1], a * b[2]);
}
Vector operator*(const Vector &a, const double b)
{
    return Vector(a[0] * b, a[1] * b, a[2] * b);
}
Vector operator/(const Vector &a, const double b)
{
    return Vector(a[0] / b, a[1] / b, a[2] / b);
}
double dot(const Vector &a, const Vector &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
Vector cross(const Vector &a, const Vector &b)
{
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}
bool operator==(const Vector &a, const Vector &b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}
bool operator!=(const Vector &a, const Vector &b)
{
    return a[0] != b[0] || a[1] != b[1] || a[2] != b[2];
}
std::ostream &operator<<(std::ostream &os, const Vector &v)
{
    os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return os;
}

double distance(const Vector &a, const Vector &b)
{
    return (a - b).norm();
}
