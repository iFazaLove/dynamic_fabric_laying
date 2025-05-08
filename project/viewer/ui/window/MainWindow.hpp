#pragma once
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "viewer/ClothViewer.hpp"
#include "../simulation/SimulationSettingsDialog.hpp"
#include "../camera/CameraSettingsDialog.hpp"

class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onConfigureClicked();   // M
    void onResetClicked();      // R
    void onStopClicked();       // S
    void onResumeClicked();     // Space
    void onSettingsRequested(); // пункт меню
    void onCameraRequested();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createMenu();
    ClothViewer* viewer;
    QPushButton* btnConfigure;
    QPushButton* btnResume;
    QPushButton* btnReset;
    QPushButton* btnStop;
};