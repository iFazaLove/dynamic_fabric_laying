#define GL_SILENCE_DEPRECATION

#include "ClothViewer.hpp"

ClothViewer::ClothViewer(QWidget *parent)
    : QOpenGLWidget(parent),
      gridRows(20),
      gridCols(20),
      physParams(::ClothParams()), // default конструктор
      objParams{Vec3(.5, .5, -0.4), 0.4},
      cloth(gridRows, gridCols, physParams),
      object(objParams.center, objParams.radius),
      // cameraFovY(camParams.fovY)
      cameraFovY(45.0f)
{
    cameraTarget = object.getCenter();
    camDistance = objParams.radius * 4.0;
    camAzimuth = M_PI / 4.0;   // 45°
    camElevation = M_PI / 6.0; // 30°
    updateCameraVec();

    timer.setInterval(16);
    connect(&timer, &QTimer::timeout, this, &ClothViewer::updateSimulation);
    //timer.start();
}

void ClothViewer::startSimulation() { if (!timer.isActive()) timer.start(); }
void ClothViewer::stopSimulation() { if (timer.isActive()) timer.stop(); }

std::pair<int, int> ClothViewer::getGridParams() const { return {gridRows, gridCols}; }
::ClothParams ClothViewer::getClothParams() const { return physParams; }
ClothViewer::ObjectParams ClothViewer::getObjectParams() const { return objParams; }
ClothViewer::CameraParams ClothViewer::getCameraParams() const
{
    return {camDistance, camAzimuth, camElevation, cameraFovY};
}

void ClothViewer::applyParameters(int rows, int cols,
                                  const ::ClothParams &cparams,
                                  const ObjectParams &oparams)
{
    bool wasRunning = timer.isActive();
    timer.stop();

    gridRows = rows; gridCols = cols; physParams = cparams; objParams = oparams;
    cloth = Cloth(gridRows, gridCols, physParams);
    object = Object(objParams.center, objParams.radius);

    cameraTarget = objParams.center;
    updateCameraVec();

    update();
    if (wasRunning)
        timer.start();
}

void ClothViewer::setCameraParams(const CameraParams &params)
{
    camDistance = params.distance;
    camAzimuth = params.azimuth;
    camElevation = params.elevation;
    cameraFovY = params.fovY;
    updateCameraVec();
    resizeGL(width(), height());
    update();
}

void ClothViewer::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glPointSize(4.0f);
    glClearColor(1, 1, 1, 1); // белый фон
}

void ClothViewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cameraFovY, float(w) / float(h ? h : 1), 0.01, 100.0);
}

void ClothViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x,    cameraPos.y,    cameraPos.z,
              cameraTarget.x, cameraTarget.y, cameraTarget.z,
              0.0,            0.0,            1.0);
    drawObject();
    drawCloth();
}

void ClothViewer::updateSimulation()
{
    cloth.step(dt, object);
    update();
}

void ClothViewer::drawObject()
{
    glColor3f(0.9f, 0.7f, 0.3f);
    glPushMatrix();
    Vec3 c = object.getCenter();
    glTranslatef(c.x, c.y, c.z);
    GLUquadric *quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);
    gluSphere(quad, object.getRadius(), 30, 30);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void ClothViewer::drawCloth()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(1.0f); glBegin(GL_LINES);
    for (auto &pr : cloth.getPoints()) {
        const auto &p = pr.second;
        for (int i = 0; i < 4; i++) {
            auto n = p.neighbors[i]; if (!n) continue;
            int nid = cloth.index(n->x, n->y); if (nid <= pr.first) continue;
            glVertex3f(p.pos.x, p.pos.y, p.pos.z);
            glVertex3f(n->pos.x, n->pos.y, n->pos.z);
        }
    }
    glEnd();
    glColor3f(1, 0, 0); glBegin(GL_POINTS);
    for (auto &pr : cloth.getPoints())
        glVertex3f(pr.second.pos.x, pr.second.pos.y, pr.second.pos.z);
    glEnd();
}

void ClothViewer::updateCameraVec()
{
    double x = camDistance * std::cos(camElevation) * std::cos(camAzimuth);
    double y = camDistance * std::cos(camElevation) * std::sin(camAzimuth);
    double z = camDistance * std::sin(camElevation);
    cameraPos = cameraTarget + Vec3(float(x), float(y), float(z));
}

void ClothViewer::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mouseLeftDown = true;
        lastMousePos = e->pos();
    }
    QOpenGLWidget::mousePressEvent(e);
}

void ClothViewer::mouseMoveEvent(QMouseEvent *e)
{
    if (!mouseLeftDown)
    {
        QOpenGLWidget::mouseMoveEvent(e);
        return;
    }
    QPoint delta = e->pos() - lastMousePos;
    lastMousePos = e->pos();
    const double sens = 0.005;
    camAzimuth -= delta.x() * sens;
    camElevation += delta.y() * sens;
    camElevation = std::clamp(camElevation, -M_PI / 2 + 0.01, M_PI / 2 - 0.01);
    updateCameraVec();
    update();
    QOpenGLWidget::mouseMoveEvent(e);
}

void ClothViewer::wheelEvent(QWheelEvent *e)
{
    double zoomStep = e->angleDelta().y() * 0.001 * camDistance;
    camDistance = std::max(0.1, camDistance - zoomStep);
    updateCameraVec();
    update();
    QOpenGLWidget::wheelEvent(e);
}