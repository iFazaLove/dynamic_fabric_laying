#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "Vec/Vec3.hpp"
#include "Object/Object.hpp"
#include "utils/log.hpp"

// Для читаемости индексов соседей
enum NeighborType
{
    LEFT = 0,
    RIGHT,
    DOWN,
    UP,
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_LEFT,
    UP_RIGHT,
    LEFT2,
    RIGHT2,
    DOWN2,
    UP2
};

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
    Point *neighbors[12]; // указатели на соседей (см. NeighborType)
};

struct ClothParams
{
    double real_width = 1.0;          // реальные размеры ткани (X)
    double real_height = 1.0;         // реальные размеры ткани (Y)
    double surface_density = 4.0;     // плотность ткани (кг/м^2)
    double spring_k = 150.0;          // коэффициент жесткости пружины
    double spring_damping = 3.0;      // коэффициент упругости пружины
    double global_damping = 0.2;      // коэффициент демпфирования пружины
    double stretch_limit = 1.5;       // предел растяжения
    double friction_coeff = 0.2;      // коэффициент трения
    double corner_k = 5.0;            // коэффициент жесткости (углы)
    double edge_k = 3.0;              // коэффициент жесткости (края)
    double normal_k = 1.0;            // коэффициент жесткости (остальные)
    Vec3 gravity = Vec3(0, 0, -9.81); // сила тяжести
};

class Cloth
{
public:
    Cloth(int width, int height, const ClothParams &params = ClothParams());
    void step(double dt, const Object &collisionSphere);

    Point &getPoint(int x, int y);
    void exportCSV(int step) const;

    int index(int x, int y) const;

    const std::map<int, Point> &getPoints() const;
    int getRows() const { return w; }
    int getCols() const { return h; }

    void fixPoint(int x, int y, bool fixed = true);

private:
    int w, h; // сетка: w × h
    ClothParams params;

    std::map<int, Point> pts; // Рекомендуется заменить на std::vector<Point> для ускорения доступа

    void initPoints();
    void linkNeighbors();

    void applyForces();
    void integrate(double dt);
    void applySpring(Point &a, Point &b, double k_base, double restLength);
    void finalizeVelocities(double dt);
    void handleSphereCollision(Point &p, const Object &sphere);

    double edge_factor(int i, int j) const; // вернуть double вместо int
    Vec3 positionFromUV(double u, double v) const;
    double getPointMass(int x, int y, Point &p) const;
};