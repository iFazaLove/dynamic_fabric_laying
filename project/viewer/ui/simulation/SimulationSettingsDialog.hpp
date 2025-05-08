#pragma once
#include <QDialog>
#include <QTabWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include "Fabric/Cloth.hpp"
#include "Vec/Vec3.hpp"
#include "viewer/ClothViewer.hpp" // for ObjectParams

class QSpinBox;
class QDoubleSpinBox;
class QDialogButtonBox;

class SimulationSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SimulationSettingsDialog(QWidget *parent = nullptr);

    void setGridParams(int rows, int cols);
    std::pair<int, int> gridParams() const;

    void setClothParams(const ::ClothParams &p);
    ::ClothParams clothParams() const;

    void setObjectParams(const ClothViewer::ObjectParams &o);
    ClothViewer::ObjectParams objectParams() const;

private:
    QTabWidget *tabs;

    // Geometry tab
    QWidget *geomTab;
    QSpinBox *sbRows;
    QSpinBox *sbCols;

    // Physical tab
    QWidget *physTab;
    QDoubleSpinBox *dsbWidth;
    QDoubleSpinBox *dsbHeight;
    QDoubleSpinBox *dsbDensity;
    QDoubleSpinBox *dsbSpringK;
    QDoubleSpinBox *dsbSpringD;
    QDoubleSpinBox *dsbGlobalD;
    QDoubleSpinBox *dsbStretchLimit;
    QDoubleSpinBox *dsbFriction;
    QDoubleSpinBox *dsbCornerK;
    QDoubleSpinBox *dsbEdgeK;
    QDoubleSpinBox *dsbNormalK;
    QDoubleSpinBox *dsbGx;
    QDoubleSpinBox *dsbGy;
    QDoubleSpinBox *dsbGz;

    // Object tab
    QWidget *objTab;
    QDoubleSpinBox *dsbCx;
    QDoubleSpinBox *dsbCy;
    QDoubleSpinBox *dsbCz;
    QDoubleSpinBox *dsbRadius;

    QDialogButtonBox *buttonBox;
};