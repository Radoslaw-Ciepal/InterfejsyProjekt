#ifndef TRANSFORMATION_DOCK_HPP
#define TRANSFORMATION_DOCK_HPP

#include "widgets/RotationDials.hpp"
#include "widgets/TransformationSliders.hpp"
#include <QDockWidget>
#include <QMatrix4x4>
#include <QVector3D>

class TransformationDock : public QDockWidget {
  Q_OBJECT
  TransformationSliders *m_translation;
  RotationDials *m_rotation;
  TransformationSliders *m_scale;

private slots:
	void onValueChanged();

signals:
	void valueChanged(QMatrix4x4);

public:
  explicit TransformationDock(QWidget *parent);
  explicit TransformationDock();

  [[nodiscard]] QVector3D translation() const;
  [[nodiscard]] QVector3D rotation() const;
  [[nodiscard]] QVector3D scale() const;
  [[nodiscard]] QMatrix4x4 transformation() const;

  void setTranslation(const QVector3D &vec3);
  void setRotation(const QVector3D &vec3);
  void setScale(const QVector3D &vec3);
};

#endif
