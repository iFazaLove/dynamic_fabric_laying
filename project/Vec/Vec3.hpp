#pragma once

struct Vec3
{
    double x, y, z;

    Vec3();
    Vec3(double x_, double y_, double z_);

    Vec3 operator+(const Vec3 &v) const;
    Vec3 operator-(const Vec3 &v) const;
    Vec3 operator*(double s) const;
    Vec3 operator/(double s) const;
    Vec3 &operator+=(const Vec3 &v);
    Vec3 &operator-=(const Vec3 &v);

    double length() const;
    Vec3 normalized() const;
    double dot(const Vec3 &other) const;
};