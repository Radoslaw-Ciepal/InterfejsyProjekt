#include "widgets/ProjectionCanvas.hpp"
#include <QtGui/qmatrix4x4.h>
#include <QtMath>
#include <memory>

ProjectionCanvas::ProjectionCanvas(QWidget *parent)
    : QWidget(parent), m_model(nullptr),
      m_transformationMatrix(std::make_shared<QMatrix4x4>()) {
  setBackgroundRole(QPalette::ColorRole::Base);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
}

ProjectionCanvas::ProjectionCanvas() : ProjectionCanvas(nullptr) {}

[[nodiscard]] float ProjectionCanvas::aspectRatio() const {
  return float(width()) / float(height());
}

void ProjectionCanvas::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);
  drawModel(painter, axis_model, false);
  if (m_model == nullptr) {
    return;
  }
  drawModel(painter, *m_model, true);
}

void ProjectionCanvas::drawModel(QPainter &painter, ModelView model,
                                 bool applyTransformations) {
  for (const auto &edge : model) {
    drawEdge(painter, edge, applyTransformations);
  }
}

void ProjectionCanvas::drawEdge(QPainter &painter, const Edge &edge,
                                bool applyTransformations) {
  painter.setPen({edge.color, 1});
  painter.drawLine(project(edge.a, applyTransformations),
                   project(edge.b, applyTransformations));
}

void ProjectionCanvas::setModel(std::shared_ptr<Model> model) {
  m_model = std::move(model);
};

void ProjectionCanvas::setTransformationMatrix(
    std::shared_ptr<QMatrix4x4> transformationMatrix) {
  m_transformationMatrix = std::move(transformationMatrix);
  update();
};

void ProjectionCanvas::setMVP(QMatrix4x4 modelViewMatrix,
                              QMatrix4x4 projectionMatrix) {
  m_modelViewMatrix = modelViewMatrix;
  m_projectionMatrix = projectionMatrix;
  update();
}

[[nodiscard]] QPoint
ProjectionCanvas::project(const QVector3D &vec3,
                          bool applyTransformations) const {
  return (applyTransformations ? m_transformationMatrix->map(vec3) : vec3)
      .project(m_modelViewMatrix, m_projectionMatrix, rect())
      .toPoint();
}
