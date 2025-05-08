#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <cassert>
#include <cmath>
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "Fabric/Cloth.hpp"
#include "Object/Object.hpp"
#include "Vec/Vec3.hpp"

class ClothViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ClothViewer(QWidget *parent = nullptr);

    // управление симуляцией
    void startSimulation();
    void stopSimulation();

    // параметры ткани и объекта
    struct ObjectParams { Vec3 center; double radius; };

    // параметры камеры
    struct CameraParams {
        double distance;
        double azimuth;
        double elevation;
        float fovY;
    };

    // получение текущих параметров симуляции
    std::pair<int, int> getGridParams()   const; // {rows, cols}
    ::ClothParams       getClothParams()  const;      // физические параметры ткани
    ObjectParams        getObjectParams() const;      // центр и радиус сферы
    CameraParams        getCameraParams() const;

    // установить новые параметры и пересоздать cloth/object
    void applyParameters(int rows,
                        int cols,
                        const ::ClothParams &cparams,
                        const ObjectParams &oparams);
    void setCameraParams(const CameraParams &camParams);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *e)override;
    void mouseMoveEvent(QMouseEvent *e)override;
    void wheelEvent(QWheelEvent *e)override;

private slots:
    void updateSimulation();

private:
    // вспомогательные отрисовщики
    void drawObject();
    void drawCloth();
    void updateCameraVec();

    // текущие параметры (отдельно, чтобы было просто отдавать в диалог)
    int           gridRows;
    int           gridCols;
    ::ClothParams physParams;
    ObjectParams  objParams;
    CameraParams  camParams;

    // собственно объекты симуляции
    Cloth  cloth;
    Object object;
    QTimer timer;
    double dt = 0.0001;

    // камера
    Vec3 cameraPos;
    Vec3 cameraTarget;
    float cameraFovY;
    double camDistance;
    double camAzimuth;
    double camElevation;

    // мышь
    bool mouseLeftDown = false;
    QPoint lastMousePos;
};