#include "Vec3.hpp"
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

Vec3 Vec3::operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
Vec3 Vec3::operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 Vec3::operator*(double s) const { return Vec3(x * s, y * s, z * s); }
Vec3 Vec3::operator/(double s) const
{
    if (s != 0)
        return Vec3(x / s, y / s, z / s);
    else
        return Vec3(0, 0, 0); // или выбросить исключение
}
Vec3 &Vec3::operator+=(const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Vec3 &Vec3::operator-=(const Vec3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

double Vec3::length() const { return sqrt(x * x + y * y + z * z); }
Vec3 Vec3::normalized() const
{
    double l = length();
    return (l > 0) ? (*this) * (1.0 / l) : Vec3();
}
double Vec3::dot(const Vec3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}