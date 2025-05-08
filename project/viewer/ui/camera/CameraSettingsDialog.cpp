#include "CameraSettingsDialog.hpp"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    spDistance = new QDoubleSpinBox(this);
    spDistance->setRange(0.1, 100.0);
    spAzimuth = new QDoubleSpinBox(this);
    spAzimuth->setRange(-360, 360);
    spElevation = new QDoubleSpinBox(this);
    spElevation->setRange(-89.9, 89.9);

    spFov = new QDoubleSpinBox(this);
    spFov->setRange(1, 179);

    QFormLayout *form = new QFormLayout(this);
    form->addRow(tr("Distance:"), spDistance);
    form->addRow(tr("Azimuth:"), spAzimuth);
    form->addRow(tr("Elevation:"), spElevation);
    form->addRow(tr("Field of view:"), spFov);

    btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(btnBox, &QDialogButtonBox::accepted, this, &CameraSettingsDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &CameraSettingsDialog::reject);
    form->addRow(btnBox);
    setLayout(form);
    setWindowTitle(tr("Camera Settings"));
}

void CameraSettingsDialog::setCameraParams(const ClothViewer::CameraParams &cam)
{
    spDistance->setValue(cam.distance);
    spAzimuth->setValue(cam.azimuth * 180.0 / M_PI);
    spElevation->setValue(cam.elevation * 180.0 / M_PI);
    spFov->setValue(cam.fovY);
}

ClothViewer::CameraParams CameraSettingsDialog::cameraParams() const
{
    return {
        spDistance->value(),
        spAzimuth->value() * M_PI / 180.0,
        spElevation->value() * M_PI / 180.0,
        float(spFov->value())};
}