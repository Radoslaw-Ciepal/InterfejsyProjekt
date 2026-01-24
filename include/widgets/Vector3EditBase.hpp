#ifndef VECTOR3_EDIT_BASE_HPP
#define VECTOR3_EDIT_BASE_HPP

#include <QGroupBox>
#include <QVector3D>

class Vector3EditBase : public QGroupBox {
  Q_OBJECT

signals:
  void valueChanged(QVector3D);

public:
  using QGroupBox::QGroupBox;
};

#endif
