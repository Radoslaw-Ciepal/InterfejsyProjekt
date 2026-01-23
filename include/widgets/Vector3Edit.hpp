#ifndef VECTOR3_EDIT_HPP
#define VECTOR3_EDIT_HPP

#include "Vector3EditBase.hpp"
#include <QGroupBox>
#include <QVector3D>

template <typename Edit, typename Layout>
class Vector3Edit : public Vector3EditBase {
  Edit *m_editX;
  Edit *m_editY;
  Edit *m_editZ;
  void updateVector() { emit valueChanged(getVector()); }

public:
  explicit Vector3Edit(const QString &title, QWidget *parent)
      : Vector3EditBase(title, parent), m_editX(new Edit("X", this)),
        m_editY(new Edit("Y", this)), m_editZ(new Edit("Z", this)) {
    auto *mainLayout = new Layout(this);

    mainLayout->addWidget(m_editX);
    mainLayout->addWidget(m_editY);
    mainLayout->addWidget(m_editZ);

    connect(m_editX, &Edit::valueChanged, this, &Vector3Edit::updateVector);
    connect(m_editY, &Edit::valueChanged, this, &Vector3Edit::updateVector);
    connect(m_editZ, &Edit::valueChanged, this, &Vector3Edit::updateVector);
  }

  explicit Vector3Edit(const QString &title) : Vector3Edit(title, nullptr) {}

  [[nodiscard]] QVector3D getVector() const {
    return {float(m_editX->value()), float(m_editY->value()),
            float(m_editZ->value())};
  }

  void setVector(const QVector3D &vec3) {
    m_editX->setValue(int(vec3.x()));
    m_editY->setValue(int(vec3.y()));
    m_editZ->setValue(int(vec3.z()));
  }
};

#endif
