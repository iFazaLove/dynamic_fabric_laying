#pragma once
#include "../Vec/Vec3.hpp"

class Object
{
public:
    Vec3 center;
    double radius;

    Object(Vec3 c, double r) : center(c), radius(r) {};

    bool inside(const Vec3 &p) const;
    Vec3 project(const Vec3 &p) const;
};