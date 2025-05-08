#include "Cloth.hpp"

Cloth::Cloth(int width, int height, const ClothParams &p)
    : w(width), h(height), params(p)
{
    initPoints();
    linkNeighbors();
}
int Cloth::index(int x, int y) const
{
    return y * w + x;
}

const std::map<int, Point> &Cloth::getPoints() const
{
    return pts;
}

void Cloth::fixPoint(int x, int y, bool fixed)
{
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;

    pts[index(x, y)].fixed = fixed;
}

void Cloth::initPoints()
{
    bool is_corner, is_edge;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            is_corner = (x == 0 || x == w - 1) && (y == 0 || y == h - 1);
            is_edge = (x == 0 || x == w - 1 || y == 0 || y == h - 1);

            Point p;
            p.x = x;
            p.y = y;
            p.u = static_cast<double>(x) / (w - 1);
            p.v = static_cast<double>(y) / (h - 1);
            p.pos = positionFromUV(p.u, p.v); // вычисляем позицию
            p.vel = Vec3(0, 0, 0);
            p.vel_half = Vec3(0, 0, 0); // стартовая скорость на полшага
            p.force = Vec3(0, 0, 0);
            p.mass = getPointMass(x, y, p); // вычисляем массу точки
            pts[index(x, y)] = p;
        }
    }
}
void Cloth::linkNeighbors()
{
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int i = index(x, y);
            Point &p = pts[i];
            for (int n = 0; n < 12; ++n)
                p.neighbors[n] = nullptr;

// Используем enum NeighborType для читаемости
            if (x > 0)
                p.neighbors[LEFT] = &pts[index(x - 1, y)];
            if (x < w - 1)
                p.neighbors[RIGHT] = &pts[index(x + 1, y)];
            if (y > 0)
                p.neighbors[DOWN] = &pts[index(x, y - 1)];
            if (y < h - 1)
                p.neighbors[UP] = &pts[index(x, y + 1)];

            if (x > 0 && y > 0)
                p.neighbors[DOWN_LEFT] = &pts[index(x - 1, y - 1)];
            if (x < w - 1 && y > 0)
                p.neighbors[DOWN_RIGHT] = &pts[index(x + 1, y - 1)];
            if (x > 0 && y < h - 1)
                p.neighbors[UP_LEFT] = &pts[index(x - 1, y + 1)];
            if (x < w - 1 && y < h - 1)
                p.neighbors[UP_RIGHT] = &pts[index(x + 1, y + 1)];

            if (x > 1)
                p.neighbors[LEFT2] = &pts[index(x - 2, y)];
            if (x < w - 2)
                p.neighbors[RIGHT2] = &pts[index(x + 2, y)];
            if (y > 1)
                p.neighbors[DOWN2] = &pts[index(x, y - 2)];
            if (y < h - 2)
                p.neighbors[UP2] = &pts[index(x, y + 2)];
        }
    }
}

void Cloth::applySpring(Point &a, Point &b, double k_base, double restLength)
{
    Vec3 delta = b.pos - a.pos;
    double len = delta.length();
    if (len == 0.0)
        return;
    Vec3 dir = delta.normalized();
    double diff = len - restLength;

    if (len > restLength * params.stretch_limit)
    {
        LOG("Stretch limit exceeded between (" << a.x << "," << a.y << ") and (" << b.x << "," << b.y << ")");

        len = restLength * params.stretch_limit;
        diff = len - restLength;
        delta = dir * len; // пересчитываем вектор, чтобы сила была согласованной
    }

    // edge_factor теперь возвращает double
    double kmod = std::max(edge_factor(a.y, a.x), edge_factor(b.y, b.x)); // y — это i
    double k = k_base * kmod;

    // Сила упругости
    Vec3 force = dir * (k * diff);

    // Демпфирование
    Vec3 dv = b.vel - a.vel;
    double v_rel = dv.dot(dir); // относительная скорость вдоль пружины
    Vec3 damp_force = dir * (params.spring_damping * v_rel);

    // Итоговая сила
    Vec3 total_force = force + damp_force;

    a.force += total_force;
    b.force -= total_force;
}

void Cloth::applyForces()
{
    for (auto &[idx, p] : pts)
    {
        int id = idx;
        p.force = Vec3(0, 0, 0);
        p.force += params.gravity * p.mass;
        p.force += p.vel * (-params.global_damping);

        double dx = params.real_width / (w - 1);
        double dy = params.real_height / (h - 1);

        for (int i = 0; i < 12; ++i)
        {
            Point *n = p.neighbors[i];
            if (!n)
                continue;

            // вычисляем уникальный номер соседа
            int nid = index(n->x, n->y);
            // если сосед уже «меньше» текущей точки — пропускаем,
            // иначе обработаем здесь единожды
            if (nid <= id)
                continue;

            double rest = 0.0;

            if (i == 0 || i == 1) // соседи слева/справа
                rest = dx;
            else if (i == 2 || i == 3) // соседи снизу/сверху
                rest = dy;
            else if (i >= 4 && i <= 7) // диагональные соседи
                rest = sqrt(dx * dx + dy * dy);
            else if (i == 8 || i == 9) // через две точки по x
                rest = 2.0 * dx;
            else if (i == 10 || i == 11) // через две точки по y
                rest = 2.0 * dy;

            applySpring(p, *n, params.spring_k, rest);
        }
    }
}

void Cloth::integrate(double dt)
{
    for (auto &[_, p] : pts)
    {
        if (p.fixed)
            continue; // пропускаем фиксированные точки

        Vec3 acc = p.force / p.mass;

        // шаг скорости на полшага
        p.vel_half += acc * (0.5 * dt);

        // шаг позиции
        p.pos += p.vel_half * dt;
    }
}

void Cloth::finalizeVelocities(double dt)
{
    for (auto &[_, p] : pts)
    {
        Vec3 acc_new = p.force / p.mass;
        p.vel_half += acc_new * (0.5 * dt);
        p.vel = p.vel_half + acc_new * (0.5 * dt);
    }
}

void Cloth::handleSphereCollision(Point &p, const Object &sphere)
{
    if (p.fixed)
        return; // пропускаем фиксированные точки

    Vec3 delta = p.pos - sphere.center;
    double dist = delta.length();

    if (dist < sphere.radius)
    {
        Vec3 n = delta.normalized();
        double penetration = sphere.radius - dist;

        LOG("Collision at point (" << p.x << "," << p.y << "), penetration: " << penetration);

        p.pos += n * penetration;

        double v_n = p.vel_half.dot(n);
        if (v_n < 0.0)
            p.vel_half -= n * v_n;

        Vec3 tangent = p.vel_half - n * p.vel_half.dot(n);
        p.vel_half -= tangent * params.friction_coeff; // трение
    }
}

void Cloth::step(double dt, const Object &sphere)
{
    applyForces(); // a(t)
    integrate(dt); // vel_half += 0.5 * a(t) * dt; pos += vel_half * dt;

    for (auto &[_, p] : pts)
        handleSphereCollision(p, sphere);

    applyForces();          // a(t + dt)
    finalizeVelocities(dt); // vel_half += 0.5 * a(t + dt) * dt;
}

double Cloth::edge_factor(int i, int j) const
{
    bool is_corner = (i == 0 || i == h - 1) && (j == 0 || j == w - 1);
    bool is_edge = (i == 0 || i == h - 1 || j == 0 || j == w - 1);
    if (is_corner)
        return params.corner_k;
    if (is_edge)
        return params.edge_k;
    return params.normal_k;
}

Vec3 Cloth::positionFromUV(double u, double v) const
{
    double real_x = u * params.real_width;
    double real_y = v * params.real_height;
    double real_z = 0.0;
    return Vec3(real_x, real_y, real_z);
}

Point &Cloth::getPoint(int x, int y)
{
    return pts[index(x, y)];
}

double Cloth::getPointMass(int x, int y, Point &p) const
{
    double dx = params.real_width / (w - 1);
    double dy = params.real_height / (h - 1);
    double cell_area = dx * dy;

    double corner_mass = .9;
    double edge_mass = .95;

    bool is_corner = (x == 0 || x == w - 1) &&
                     (y == 0 || y == h - 1);
    bool is_edge = (x == 0 || x == w - 1 ||
                    y == 0 || y == h - 1);

    if (is_corner)
        return params.surface_density * cell_area * corner_mass;
    else if (is_edge)
        return params.surface_density * cell_area * edge_mass;
    return params.surface_density * cell_area;
}

void Cloth::exportCSV(int step) const
{
    // Создаём директорию, если не существует
    std::filesystem::create_directories("output");

    // Имя файла
    std::ostringstream fname;
    fname << "output/cloth_step_" << step << ".csv";

    std::ofstream out(fname.str());
    if (!out.is_open())
    {
        std::cerr << "Cannot open file for writing: " << fname.str() << "\n";
        return;
    }

    out << "x,y,z\n"; // заголовок

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const Point &p = pts.at(index(x, y));
            out << p.pos.x << "," << p.pos.y << "," << p.pos.z << "\n";
        }
    }

    out.close();
}
