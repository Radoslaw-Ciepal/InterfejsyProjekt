#ifndef PROJECTION_CANVAS_HPP
#define PROJECTION_CANVAS_HPP

#include "Model.hpp"
#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>
#include <QWidget>
#include <QtGui>

class ProjectionCanvas : public QWidget {
  Q_OBJECT
  std::shared_ptr<Model> m_model;
  std::shared_ptr<QMatrix4x4> m_transformationMatrix;
  QMatrix4x4 m_modelViewMatrix;
  QMatrix4x4 m_projectionMatrix;

  void paintEvent(QPaintEvent *event) override;
  void drawModel(QPainter &, ModelView, bool);
  void drawEdge(QPainter &, const Edge &, bool);
  [[nodiscard]] QPoint project(const QVector3D &, bool) const;

public slots:
  void setModel(std::shared_ptr<Model>);
  void setTransformationMatrix(std::shared_ptr<QMatrix4x4>);
  void setMVP(QMatrix4x4, QMatrix4x4);

public:
  explicit ProjectionCanvas(QWidget *);
  explicit ProjectionCanvas();

  [[nodiscard]] float aspectRatio() const;
};

#endif
