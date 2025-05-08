#include "ui/window/MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;
    win.resize(900, 700);
    win.show();
    return app.exec();
}
