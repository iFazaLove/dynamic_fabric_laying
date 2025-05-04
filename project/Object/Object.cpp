#include "Object.hpp"

bool Object::inside(const Vec3 &p) const
{
    return (p - center).length() < radius;
}

Vec3 Object::project(const Vec3 &p) const
{
    Vec3 dir = (p - center).normalized();
    return center + dir * radius;
}
