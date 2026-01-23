#include "docks/ProjectionDock.hpp"
#include "ProjectionMode.hpp"
#include "widgets/ProjectionCanvas.hpp"
#include <QComboBox>
#include <QMatrix4x4>
#include <QVBoxLayout>
#include <utility>

ProjectionDock::ProjectionDock(QWidget *parent) : QDockWidget("Rzut", parent) {
  auto *mainWidget = new QWidget(this);
  auto *mainLayout = new QVBoxLayout(mainWidget);
  setWidget(mainWidget);

  m_projectionCanvas = new ProjectionCanvas(mainWidget);
  mainLayout->addWidget(m_projectionCanvas);
  connect(this, &ProjectionDock::stateChanged, m_projectionCanvas,
          &ProjectionCanvas::setMVP);

  m_modeComboBox = new QComboBox(mainWidget);
  mainLayout->addWidget(m_modeComboBox);
  connect(m_modeComboBox, &QComboBox::currentIndexChanged, this,
          &ProjectionDock::onStateChanged);
  for (const auto *label : ModeLabels) {
    m_modeComboBox->addItem(QString(label));
  }

  onStateChanged();
}

ProjectionDock::ProjectionDock() : ProjectionDock(nullptr) {}

void ProjectionDock::setModel(std::shared_ptr<Model> model) {
  m_projectionCanvas->setModel(std::move(model));
}

void ProjectionDock::setTransformationMatrix(
    std::shared_ptr<QMatrix4x4> transformationMatrix) {
  m_projectionCanvas->setTransformationMatrix(std::move(transformationMatrix));
}

QMatrix4x4 ProjectionDock::projectionMatrix() const {
  QMatrix4x4 matrix;
  auto mode = ProjectionMode(m_modeComboBox->currentIndex());
  if (mode == ProjectionMode::Perspective) {
    matrix.perspective(90, m_projectionCanvas->aspectRatio(), 0.1F, 100);
  } else if (mode == ProjectionMode::Ortho ||
             mode == ProjectionMode::Isometric) {
    matrix.ortho(-2 * m_projectionCanvas->aspectRatio(),
                 2 * m_projectionCanvas->aspectRatio(), -2, 2, -2, 2);
  } else if (mode == ProjectionMode::Oblique) {
    matrix.setColumn(2, QVector4D(qCos(alpha), qSin(alpha), 0, 0));
    QMatrix4x4 temp;
    temp.scale(0.5f / m_projectionCanvas->aspectRatio(), 0.5, 1);
    matrix = temp * matrix;
    matrix.translate(0.5, 0.5, 0);
  }
  return matrix;
  // case ProjectionMode::ObliqueCabinet:{
  //   projectionMatrix.setColumn(
  //       2, QVector4D(0.5f * qCos(alpha), 0.5f * qSin(alpha), 0, 0));
  //   QMatrix4x4 temp;
  //   temp.scale(0.5f / aspectRatio(), 0.5, 1);
  //   projectionMatrix = temp * projectionMatrix;
  //   projectionMatrix.translate(0.5, 0.5, 0);
  // } break;
}

QMatrix4x4 ProjectionDock::modelViewMatrix() const {
  QMatrix4x4 matrix;
  auto mode = ProjectionMode(m_modeComboBox->currentIndex());

  if (mode == ProjectionMode::Perspective) {
    matrix.lookAt(QVector3D(0, 5, 5), QVector3D(0, 0, 0), QVector3D(0, 0, -1));
  } else if (mode == ProjectionMode::Ortho) {
    matrix.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, -1, 0));
  }
  // case ProjectionMode::OrthoDown:
  //   modelViewMatrix.lookAt(QVector3D(0, 0, -5), QVector3D(0, 0, 0),
  //                          QVector3D(0, -1, 0));

  // case ProjectionMode::OrthoFront:
  //   modelViewMatrix.lookAt(QVector3D(0, 5, 0), QVector3D(0, 0, 0),
  //                          QVector3D(0, 0, -1));
  else if (mode == ProjectionMode::Oblique) {
    matrix.lookAt(QVector3D(0, 1, 0), QVector3D(0, 0, 0), QVector3D(0, 0, -1));
  } else if (mode == ProjectionMode::Isometric) {
    matrix.lookAt(QVector3D(2, 2, 2 * qSqrt(float(2) / 3)), QVector3D(0, 0, 0),
                  QVector3D(0, 0, -1));
  }
  return matrix;
}

void ProjectionDock::onStateChanged() {
  emit stateChanged(modelViewMatrix(), projectionMatrix());
}

void ProjectionDock::resizeEvent(QResizeEvent * /*event*/) { onStateChanged(); }
