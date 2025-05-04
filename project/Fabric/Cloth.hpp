#pragma once
#include <map>
#include "../Vec/Vec3.hpp"
#include "../Object/Object.hpp"

struct Point
{
    int x, y;             // координаты в сетке
    double u, v;          // параметрические координаты
    bool fixed = false;   // фиксированная точка (true - фиксирована)
    Vec3 pos;             // положение в пространстве (для отображения)
    Vec3 vel;             // скорость
    Vec3 vel_half;        // скорость, сдвинутая на полшага
    Vec3 force;           // результирующая сила
    double mass;          // масса точки
    Point *neighbors[12]; // указатели на соседей
};

class Cloth
{
public:
    Cloth(int width, int height);
    void step(double dt, const Object &collisionSphere);

    Point &getPoint(int x, int y);
    void exportCSV(int step) const;

protected:
    double real_width = 1.0;          // реальные размеры ткани (X)
    double real_height = 1.0;         // реальные размеры ткани (Y)
    double surface_density = 4.0;     // плотность ткани (кг/м^2)
    double spring_k = 150.0;          // коэффициент жесткости пружины
    double spring_damping = 3.0;      // коэффициент упругости пружины
    double global_damping = 0.2;      // коэффициент демпфирования пружины
    double stretch_limit = 1.5;       // предел растяжения
    double friction_coeff = 0.2;      // коэффициент трения
    double point_mass;                // масса точки
    double corner_k = 5.0;            // дополниетльные коэффициенты для жесткости (углов)
    double edge_k = 3.0;              // дополнительные коэффициенты для жесткости (края)
    double normal_k = 1.0;            // дополнительные коэффициенты для жесткости (остальные)
    Vec3 gravity = Vec3(0, 0, -9.81); // сила тяжести

private:
    int w, h; // сетка: w × h

    std::map<int, Point> pts;
    int index(int x, int y) const;

    void initPoints();
    void linkNeighbors();

    void applyForces();
    void integrate(double dt);
    void applySpring(Point &a, Point &b, double k_base, double restLength);
    void finalizeVelocities(double dt);
    void handleSphereCollision(Point &p, const Object &sphere);

    int edge_factor(int i, int j) const;
    Vec3 positionFromUV(double u, double v) const;
    double getPointMass(int x, int y, Point& p) const;
};