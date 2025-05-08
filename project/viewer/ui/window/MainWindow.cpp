#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *vlay = new QVBoxLayout(central);
    QHBoxLayout *hlay = new QHBoxLayout;

    btnConfigure = new QPushButton(tr("Configure & Start (M)"), this);
    btnReset    = new QPushButton(tr("Reset Simulation (R)"), this);
    btnStop     = new QPushButton(tr("Stop Simulation (S)"), this);
    btnResume   = new QPushButton(tr("Resume Simulation (Space)"), this);

    btnReset->setEnabled(false);
    btnResume->setEnabled(false);
    btnStop->setEnabled(false);

    btnConfigure->setShortcut(QKeySequence(Qt::Key_M));
    btnReset->setShortcut(QKeySequence(Qt::Key_R));
    btnStop->setShortcut(QKeySequence(Qt::Key_S));
    btnResume->setShortcut(QKeySequence(Qt::Key_Space));

    hlay->addWidget(btnConfigure);
    hlay->addWidget(btnReset);
    hlay->addWidget(btnStop);
    hlay->addWidget(btnResume);
    hlay->addStretch();
    vlay->addLayout(hlay);

    // Viewer
    viewer = new ClothViewer(this);
    viewer->hide();
    vlay->addWidget(viewer, 1);

    setCentralWidget(central);
    setWindowTitle(tr("Dynamic Fabric Laying"));
    resize(900, 700);

    createMenu();

    connect(btnConfigure, &QPushButton::clicked, this, &MainWindow::onConfigureClicked);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(btnResume, &QPushButton::clicked, this, &MainWindow::onResumeClicked);
    connect(btnStop, &QPushButton::clicked, this, &MainWindow::onStopClicked);
}

void MainWindow::createMenu()
{
    menuBar()->addAction(tr("Settings…"), this, &MainWindow::onSettingsRequested);
    menuBar()->addSeparator();
    menuBar()->addAction(tr("Camera…"), this, &MainWindow::onCameraRequested);

// На macOS по умолчанию меню уезжает в глобальную панель
// Чтобы оно было внутри окна — выключим native menu bar:
#ifdef Q_OS_MAC
    menuBar()->setNativeMenuBar(false);
#endif
}

void MainWindow::onConfigureClicked()
{
    SimulationSettingsDialog dlg(this);
    auto [r, c] = viewer->getGridParams();
    dlg.setGridParams(r, c);
    dlg.setClothParams(viewer->getClothParams());
    dlg.setObjectParams(viewer->getObjectParams());
    if (dlg.exec() != QDialog::Accepted)
        return;

    auto [nr, nc] = dlg.gridParams();
    auto cp = dlg.clothParams();
    auto op = dlg.objectParams();
    viewer->applyParameters(nr, nc, cp, op);
    viewer->show();
    viewer->startSimulation();

    btnConfigure->setEnabled(false);
    btnReset->setEnabled(true);
    btnResume->setEnabled(false);
    btnStop->setEnabled(true);
}

void MainWindow::onResetClicked()
{
    auto [r, c] = viewer->getGridParams();
    auto cp = viewer->getClothParams();
    auto op = viewer->getObjectParams();
    viewer->applyParameters(r, c, cp, op);
    viewer->show();
    viewer->startSimulation();

    btnConfigure->setEnabled(false);
    btnReset->setEnabled(true);
    btnResume->setEnabled(false);
    btnStop->setEnabled(true);
}

void MainWindow::onResumeClicked()
{
    viewer->startSimulation();
    btnResume->setEnabled(false);
    btnStop->setEnabled(true);
}

void MainWindow::onStopClicked()
{
    viewer->stopSimulation();
    btnConfigure->setEnabled(true);
    btnResume->setEnabled(true);
    btnStop->setEnabled(false);
}

void MainWindow::onSettingsRequested()
{
    SimulationSettingsDialog dlg(this);
    auto [r, c] = viewer->getGridParams();
    dlg.setGridParams(r, c);
    dlg.setClothParams(viewer->getClothParams());
    dlg.setObjectParams(viewer->getObjectParams());
    if (dlg.exec() == QDialog::Accepted)
    {
        auto [nr, nc] = dlg.gridParams();
        auto cp = dlg.clothParams();
        auto op = dlg.objectParams();
        viewer->applyParameters(nr, nc, cp, op);
    }
}

void MainWindow::onCameraRequested()
{
    CameraSettingsDialog dlg(this);
    auto cam = viewer->getCameraParams();
    dlg.setCameraParams(cam);
    if (dlg.exec() == QDialog::Accepted)
    {
        auto newCam = dlg.cameraParams();
        viewer->setCameraParams(newCam);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QString t = event->text();
    // Configure: M или 'Ь' (русская раскладка)
    if ((event->key() == Qt::Key_M) || t.compare(QStringLiteral("ь"), Qt::CaseInsensitive) == 0)
    {
        onConfigureClicked();
        return;
    }
    // Reset: R или 'К'
    if ((event->key() == Qt::Key_R) || t.compare(QStringLiteral("к"), Qt::CaseInsensitive) == 0)
    {
        onResetClicked();
        return;
    }
    // Stop: S или 'Ы'
    if ((event->key() == Qt::Key_S) || t.compare(QStringLiteral("ы"), Qt::CaseInsensitive) == 0)
    {
        onStopClicked();
        return;
    }
    // Resume: Space
    if (event->key() == Qt::Key_Space)
    {
        onResumeClicked();
        return;
    }

    // В остальном — дефолтная обработка
    QMainWindow::keyPressEvent(event);
}