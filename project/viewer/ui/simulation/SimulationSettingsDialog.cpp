#include "SimulationSettingsDialog.hpp"

SimulationSettingsDialog::SimulationSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    tabs = new QTabWidget(this);

    // Geometry
    geomTab = new QWidget(this);
    QFormLayout *gLay = new QFormLayout(geomTab);
    sbRows = new QSpinBox(this);
    sbRows->setRange(2, 500);
    sbCols = new QSpinBox(this);
    sbCols->setRange(2, 500);
    gLay->addRow(tr("Grid rows:"), sbRows);
    gLay->addRow(tr("Grid cols:"), sbCols);
    tabs->addTab(geomTab, tr("Geometry"));

    // Physical
    physTab = new QWidget(this);
    QFormLayout *pLay = new QFormLayout(physTab);
    dsbWidth = new QDoubleSpinBox(this);
    dsbWidth->setRange(0.01, 10.0);
    dsbWidth->setDecimals(3);
    dsbHeight = new QDoubleSpinBox(this);
    dsbHeight->setRange(0.01, 10.0);
    dsbHeight->setDecimals(3);
    dsbDensity = new QDoubleSpinBox(this);
    dsbDensity->setRange(0.1, 100.0);
    dsbDensity->setDecimals(3);
    dsbSpringK = new QDoubleSpinBox(this);
    dsbSpringK->setRange(0.0, 1e4);
    dsbSpringK->setDecimals(1);
    dsbSpringD = new QDoubleSpinBox(this);
    dsbSpringD->setRange(0.0, 1e3);
    dsbSpringD->setDecimals(1);
    dsbGlobalD = new QDoubleSpinBox(this);
    dsbGlobalD->setRange(0.0, 10.0);
    dsbGlobalD->setDecimals(3);
    dsbStretchLimit = new QDoubleSpinBox(this);
    dsbStretchLimit->setRange(1.0, 5.0);
    dsbStretchLimit->setDecimals(3);
    dsbFriction = new QDoubleSpinBox(this);
    dsbFriction->setRange(0.0, 5.0);
    dsbFriction->setDecimals(3);
    dsbCornerK = new QDoubleSpinBox(this);
    dsbCornerK->setRange(0.0, 1e3);
    dsbCornerK->setDecimals(1);
    dsbEdgeK = new QDoubleSpinBox(this);
    dsbEdgeK->setRange(0.0, 1e3);
    dsbEdgeK->setDecimals(1);
    dsbNormalK = new QDoubleSpinBox(this);
    dsbNormalK->setRange(0.0, 1e3);
    dsbNormalK->setDecimals(1);
    dsbGx = new QDoubleSpinBox(this);
    dsbGx->setRange(-50, 50);
    dsbGx->setDecimals(3);
    dsbGy = new QDoubleSpinBox(this);
    dsbGy->setRange(-50, 50);
    dsbGy->setDecimals(3);
    dsbGz = new QDoubleSpinBox(this);
    dsbGz->setRange(-50, 50);
    dsbGz->setDecimals(3);

    pLay->addRow(tr("Real width:"), dsbWidth);
    pLay->addRow(tr("Real height:"), dsbHeight);
    pLay->addRow(tr("Surface density:"), dsbDensity);
    pLay->addRow(tr("Spring k:"), dsbSpringK);
    pLay->addRow(tr("Spring damping:"), dsbSpringD);
    pLay->addRow(tr("Global damping:"), dsbGlobalD);
    pLay->addRow(tr("Stretch limit:"), dsbStretchLimit);
    pLay->addRow(tr("Friction coeff:"), dsbFriction);
    pLay->addRow(tr("Corner k:"), dsbCornerK);
    pLay->addRow(tr("Edge k:"), dsbEdgeK);
    pLay->addRow(tr("Normal k:"), dsbNormalK);
    pLay->addRow(tr("Gravity X:"), dsbGx);
    pLay->addRow(tr("Gravity Y:"), dsbGy);
    pLay->addRow(tr("Gravity Z:"), dsbGz);
    tabs->addTab(physTab, tr("Physical"));

    // Object
    objTab = new QWidget(this);
    QFormLayout *oLay = new QFormLayout(objTab);
    dsbCx = new QDoubleSpinBox(this);
    dsbCx->setRange(-10, 10);
    dsbCx->setDecimals(3);
    dsbCy = new QDoubleSpinBox(this);
    dsbCy->setRange(-10, 10);
    dsbCy->setDecimals(3);
    dsbCz = new QDoubleSpinBox(this);
    dsbCz->setRange(-10, 10);
    dsbCz->setDecimals(3);
    dsbRadius = new QDoubleSpinBox(this);
    dsbRadius->setRange(0.01, 10.0);
    dsbRadius->setDecimals(3);
    oLay->addRow(tr("Center X:"), dsbCx);
    oLay->addRow(tr("Center Y:"), dsbCy);
    oLay->addRow(tr("Center Z:"), dsbCz);
    oLay->addRow(tr("Radius:"), dsbRadius);
    tabs->addTab(objTab, tr("Object"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SimulationSettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SimulationSettingsDialog::reject);

    QVBoxLayout *mainLay = new QVBoxLayout(this);
    mainLay->addWidget(tabs);
    mainLay->addWidget(buttonBox);
    setLayout(mainLay);
    setWindowTitle(tr("Simulation Settings"));
}

void SimulationSettingsDialog::setGridParams(int rows, int cols)
{
    sbRows->setValue(rows);
    sbCols->setValue(cols);
}

std::pair<int, int> SimulationSettingsDialog::gridParams() const
{
    return {sbRows->value(), sbCols->value()};
}

void SimulationSettingsDialog::setClothParams(const ::ClothParams &p)
{
    dsbWidth->setValue(p.real_width);
    dsbHeight->setValue(p.real_height);
    dsbDensity->setValue(p.surface_density);
    dsbSpringK->setValue(p.spring_k);
    dsbSpringD->setValue(p.spring_damping);
    dsbGlobalD->setValue(p.global_damping);
    dsbStretchLimit->setValue(p.stretch_limit);
    dsbFriction->setValue(p.friction_coeff);
    dsbCornerK->setValue(p.corner_k);
    dsbEdgeK->setValue(p.edge_k);
    dsbNormalK->setValue(p.normal_k);
    dsbGx->setValue(p.gravity.x);
    dsbGy->setValue(p.gravity.y);
    dsbGz->setValue(p.gravity.z);
}

::ClothParams SimulationSettingsDialog::clothParams() const
{
    ::ClothParams p;
    p.real_width = dsbWidth->value();
    p.real_height = dsbHeight->value();
    p.surface_density = dsbDensity->value();
    p.spring_k = dsbSpringK->value();
    p.spring_damping = dsbSpringD->value();
    p.global_damping = dsbGlobalD->value();
    p.stretch_limit = dsbStretchLimit->value();
    p.friction_coeff = dsbFriction->value();
    p.corner_k = dsbCornerK->value();
    p.edge_k = dsbEdgeK->value();
    p.normal_k = dsbNormalK->value();
    p.gravity = Vec3(dsbGx->value(), dsbGy->value(), dsbGz->value());
    return p;
}

void SimulationSettingsDialog::setObjectParams(const ClothViewer::ObjectParams &o)
{
    dsbCx->setValue(o.center.x);
    dsbCy->setValue(o.center.y);
    dsbCz->setValue(o.center.z);
    dsbRadius->setValue(o.radius);
}

ClothViewer::ObjectParams SimulationSettingsDialog::objectParams() const
{
    return {Vec3(dsbCx->value(), dsbCy->value(), dsbCz->value()), dsbRadius->value()};
}