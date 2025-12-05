#include <AngleDial.h>
#include <AppWindow.h>
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QMenuBar>
#include <QVBoxLayout>
#include <TransformationSlider.h>
#include <memory>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto model = std::make_shared<Model>();
  AppWindow window(model);
  window.setWindowTitle("Rzutowanie brył");
  auto *fileMenu = window.menuBar()->addMenu("Plik");
  auto *loadModelAction =
      fileMenu->addAction("Wczytaj model...", [&window, model]() {
        *model = Model::fromGeoFile(QFileDialog::getOpenFileName(
                                        &window, QString("Wczytaj model->.."),
                                        QString(""), QString("*.geo"))
                                        .toStdString())
                     .value();
      });
  loadModelAction->setShortcut(
      QKeyCombination(Qt::KeyboardModifier::ControlModifier, Qt::Key::Key_O));

  auto *transformationsPanelLayout = new QVBoxLayout();
  transformationsPanelLayout->setSpacing(32);
  transformationsPanelLayout->setContentsMargins(16, 32, 16, 32);

  auto *transformationsPanel = new QWidget();
  transformationsPanel->setLayout(transformationsPanelLayout);

  auto *transformationsPanelDock = new QDockWidget("Przekształcenia");
  transformationsPanelDock->setWidget(transformationsPanel);

  window.addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea,
                       transformationsPanelDock);

  auto *translateSlidersGroupLayout = new QVBoxLayout();
  auto *translateSlidersGroup = new QGroupBox("Przesunięcie");
  translateSlidersGroup->setLayout(translateSlidersGroupLayout);
  transformationsPanelLayout->addWidget(translateSlidersGroup);

  using AxisSetter = decltype(&QVector3D::setX);
  const std::map<QString, AxisSetter> vectorAxes{{"X", &QVector3D::setX},
                                                 {"Y", &QVector3D::setY},
                                                 {"Z", &QVector3D::setZ}};

  for (const auto &[axis, setter] : vectorAxes) {
    auto *slider = new TransformationSlider(axis);
    slider->slider->setMinimum(-5);
    slider->slider->setMaximum(5);
    QObject::connect(slider->slider, &QSlider::valueChanged,
                     [&setter, model](int v) {
                       (model->translation.*setter)(static_cast<float>(v));
                     });
    QObject::connect(slider->slider, &QSlider::valueChanged,
                     [&window](int) { window.projectionPane->update(); });
    translateSlidersGroupLayout->addWidget(slider);
  }

  auto *rotationDialsGroup = new QGroupBox("Obrót");
  auto *rotationDialsGroupLayout = new QHBoxLayout();
  rotationDialsGroup->setLayout(rotationDialsGroupLayout);
  transformationsPanelLayout->addWidget(rotationDialsGroup);

  for (const auto &[axis, setter] : vectorAxes) {
    auto *dial = new AngleDial(axis);
    QObject::connect(dial->dial, &QDial::valueChanged,
                     [&setter, model](int v) {
                       (model->rotation.*setter)(static_cast<float>(v));
                     });
    QObject::connect(dial->dial, &QDial::valueChanged,
                     [&window](int) { window.projectionPane->update(); });
    rotationDialsGroupLayout->addWidget(dial);
  }

  auto *scaleSlidersGroup = new QGroupBox("Skalowanie");
  auto *scaleSlidersGroupLayout = new QVBoxLayout();
  scaleSlidersGroup->setLayout(scaleSlidersGroupLayout);
  transformationsPanelLayout->addWidget(scaleSlidersGroup);

  for (const auto &[axis, setter] : vectorAxes) {
    auto *slider = new TransformationSlider(axis);
    slider->slider->setMinimum(-5);
    slider->slider->setMaximum(5);
    QObject::connect(slider->slider, &QSlider::valueChanged,
                     [&setter, model](int v) {
                       (model->scale.*setter)(static_cast<float>(v));
                     });
    QObject::connect(slider->slider, &QSlider::valueChanged,
                     [&window](int) { window.projectionPane->update(); });
    scaleSlidersGroupLayout->addWidget(slider);
  }

  auto *projectionModeDropdown = new QComboBox();
  transformationsPanelLayout->addWidget(projectionModeDropdown);
  projectionModeDropdown->addItems({
      "Perspektywiczny (1 punkt zbieżności)",
      "Ortogonalny (Góra)",
      "Ortogonalny (Dół)",
      "Ortogonalny (Przód)",
      "Ukośny (Gabinetowy)",
      "Ukośny (Kawaleryjski)",
      "Izometryczny",
      "Perspektywiczny (2 punkty zbieżności)",
  });
  QObject::connect(
      projectionModeDropdown, &QComboBox::currentIndexChanged,
      [&window](int index) { window.projectionPane->mode = Mode(index); });
  QObject::connect(projectionModeDropdown, &QComboBox::currentIndexChanged,
                   [&window](int) { window.projectionPane->update(); });

  projectionModeDropdown->setCurrentIndex(7);
  transformationsPanelLayout->addStretch();

  window.setWindowTitle("Rzutowanie brył");
  window.resize(800, 600);
  window.show();
  return QApplication::exec();
};
