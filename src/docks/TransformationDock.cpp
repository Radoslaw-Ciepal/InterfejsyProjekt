#include "docks/TransformationDock.hpp"
#include "widgets/RotationDials.hpp"
#include "widgets/TransformationSliders.hpp"
#include <QWidget>

TransformationDock::TransformationDock(QWidget *parent)
    : QDockWidget("Przekształcenia", parent) {
  auto *mainWidget = new QWidget(this);
  setWidget(mainWidget);

  m_translation = new TransformationSliders("Przesunięcie", mainWidget);
  m_rotation = new RotationDials("Obrót", mainWidget);
  m_scale = new TransformationSliders("Skala", mainWidget);

  auto *mainLayout = new QVBoxLayout(mainWidget);
  mainLayout->setSpacing(32);
  mainLayout->setContentsMargins(16, 32, 16, 32);
  mainLayout->addWidget(m_translation);
  mainLayout->addWidget(m_rotation);
  mainLayout->addWidget(m_scale);
  setScale({1, 1, 1});
  
  connect(m_translation, &TransformationSliders::valueChanged, this,
          &TransformationDock::onValueChanged);
  connect(m_rotation, &RotationDials::valueChanged, this,
          &TransformationDock::onValueChanged);
  connect(m_scale, &TransformationSliders::valueChanged, this,
          &TransformationDock::onValueChanged);
}

TransformationDock::TransformationDock() : TransformationDock(nullptr) {}

[[nodiscard]] QVector3D TransformationDock::translation() const {
  return m_translation->getVector();
};

[[nodiscard]] QVector3D TransformationDock::rotation() const {
  return m_rotation->getVector();
};

[[nodiscard]] QVector3D TransformationDock::scale() const {
  return m_scale->getVector();
};

[[nodiscard]] QMatrix4x4 TransformationDock::transformation() const {
  QMatrix4x4 transformation_;
  QVector3D rotation_ = rotation();
  transformation_.scale(scale());
  transformation_.rotate(rotation_.x(), 1, 0, 0);
  transformation_.rotate(rotation_.y(), 0, 1, 0);
  transformation_.rotate(rotation_.z(), 0, 0, 1);
  transformation_.translate(translation());
  return transformation_;
}

void TransformationDock::setTranslation(const QVector3D &vec3) {
  m_translation->setVector(vec3);
};

void TransformationDock::setRotation(const QVector3D &vec3) {
  m_rotation->setVector(vec3);
};

void TransformationDock::setScale(const QVector3D &vec3) {
  m_scale->setVector(vec3);
};

void TransformationDock::onValueChanged() { emit valueChanged(transformation()); }
