#pragma once
#include <QDialog>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include "viewer/ClothViewer.hpp"

class CameraSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CameraSettingsDialog(QWidget *parent = nullptr);
    void setCameraParams(const ClothViewer::CameraParams &cam);
    ClothViewer::CameraParams cameraParams() const;

private:
    QDoubleSpinBox *spDistance;
    QDoubleSpinBox *spAzimuth;   // в градусах
    QDoubleSpinBox *spElevation; // в градусах
    QDoubleSpinBox *spFov;
    QDialogButtonBox *btnBox;
};