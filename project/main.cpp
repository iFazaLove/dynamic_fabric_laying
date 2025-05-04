#include <iostream>
#include <filesystem>
#include "Fabric/Cloth.hpp"
#include "Object/Object.hpp"
#include "Vec/Vec3.hpp"

namespace fs = std::filesystem;

void cleanOutputFolder()
{
    const std::string folder = "output";

    // Создаём папку, если нет
    if (!fs::exists(folder))
    {
        fs::create_directory(folder);
        return;
    }

    // Удаляем все .csv файлы внутри
    for (const auto &entry : fs::directory_iterator(folder))
    {
        if (entry.path().extension() == ".csv")
        {
            fs::remove(entry.path());
        }
    }
}
int main()
{
    cleanOutputFolder(); // очищаем папку вывода

    Object sphere(Vec3(.5, .5, -0.4), 0.4);

    int h = 20, w = 20;

    Cloth cloth(w, h);

    for (int i = 0; i < 6000; ++i)
    {
        cloth.step(0.0001, sphere);

        if (i % 20 == 0)
            cloth.exportCSV(i);
    }

    std::cout << "Экспорт завершён.\n";
    return 0;
}