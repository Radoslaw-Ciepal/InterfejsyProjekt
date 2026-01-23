#ifndef PROJECTION_DOCK_HPP
#define PROJECTION_DOCK_HPP

#include "widgets/ProjectionCanvas.hpp"
#include <QComboBox>
#include <QDockWidget>
#include <QMatrix4x4>
#include <numbers>

class ProjectionDock : public QDockWidget {
  Q_OBJECT
  ProjectionCanvas *m_projectionCanvas;
  QComboBox *m_modeComboBox;
  float alpha = std::numbers::pi / 4;
  float k = 0.5f;
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void onStateChanged();

public slots:
  void setModel(std::shared_ptr<Model>);
  void setTransformationMatrix(std::shared_ptr<QMatrix4x4>);

signals:
  void stateChanged(QMatrix4x4, QMatrix4x4);

public:
  explicit ProjectionDock(QWidget *parent);
  explicit ProjectionDock();

  [[nodiscard]] QMatrix4x4 modelViewMatrix() const;
  [[nodiscard]] QMatrix4x4 projectionMatrix() const;
};

#endif
