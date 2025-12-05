#include <ProjectionPane.h>
#include <QWidget>
#include <QtGui>
#include <QtMath>

ProjectionPane::ProjectionPane() {
  setBackgroundRole(QPalette::ColorRole::Base);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
};

[[nodiscard]] float ProjectionPane::aspectRatio() const {
  return static_cast<float>(width()) / static_cast<float>(height());
};

void ProjectionPane::calculateProjectionMatrix() {
  projectionMatrix.setToIdentity();
  switch (mode) {
  case Mode::PerspectiveSingle: {
    projectionMatrix.perspective(90, aspectRatio(), 0.1F, 100);
  } break;
  case Mode::OrthoUp:
  case Mode::OrthoDown:
  case Mode::OrthoFront:
  case Mode::Isometric: {
    projectionMatrix.ortho(-2 * aspectRatio(), 2 * aspectRatio(), -2, 2, -2, 2);
  } break;
  case Mode::ObliqueCabinet:
  case Mode::ObliqueCavalier: {
    projectionMatrix.setColumn(
        2, QVector4D(k * qCos(alpha), k * qSin(alpha), 0, 0));
    QMatrix4x4 temp{};
    temp.scale(0.5f / aspectRatio(), 0.5, 1);
    projectionMatrix = temp * projectionMatrix;
    projectionMatrix.translate(0.5, 0.5, 0);
  } break;
  case Mode::PerspectiveDouble: {
    const float thetaX = qDegreesToRadians(120);
    const float thetaZ = qDegreesToRadians(90);
    const float d = 3;
    projectionMatrix.setColumn(
        2, QVector4D{-1 / qTan(thetaX), -1 / qTan(thetaZ), 1, -1 / d});
    projectionMatrix.setColumn(3, QVector4D{0, 0, d, 0});
    auto temp = QMatrix4x4{};
    temp.scale(0.5f / aspectRatio(), 0.5, 1);
    projectionMatrix = temp * projectionMatrix;
    // projectionMatrix.translate(0.5, 0.5, 0);
  } break;
  }
}

void ProjectionPane::calculateModelViewMatrix() {
  modelViewMatrix.setToIdentity();
  switch (mode) {
  case Mode::PerspectiveSingle:
  case Mode::PerspectiveDouble:
    // perspektywa
    modelViewMatrix.lookAt(QVector3D{2, 2, 2}, QVector3D{0, 0, 0},
                           QVector3D{0, 0, -1});
    break;
  case Mode::OrthoUp:
    // orto góra
    modelViewMatrix.lookAt(QVector3D{0, 0, 5}, QVector3D{0, 0, 0},
                           QVector3D{0, -1, 0});
    break;
  case Mode::OrthoDown:
    // orto dół
    modelViewMatrix.lookAt(QVector3D{0, 0, -5}, QVector3D{0, 0, 0},
                           QVector3D{0, -1, 0});
    break;
  case Mode::OrthoFront:
    // orto przód
    modelViewMatrix.lookAt(QVector3D{0, 5, 0}, QVector3D{0, 0, 0},
                           QVector3D{0, 0, -1});
    break;
  case Mode::ObliqueCabinet:
  case Mode::ObliqueCavalier:
    modelViewMatrix.lookAt(QVector3D{0, 1, 0}, QVector3D{0, 0, 0},
                           QVector3D{0, 0, -1});
    break;
  case Mode::Isometric:
    // izometryczny
    modelViewMatrix.lookAt(
        QVector3D{2, 2, 2 * qSqrt(static_cast<float>(2) / 3)},
        QVector3D{0, 0, 0}, QVector3D{0, 0, -1});
    break;
  }
}

void ProjectionPane::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  calculateProjectionMatrix();
  calculateModelViewMatrix();
  drawAxis(painter);
  if (model != nullptr) {
    for (const auto &vertex : model->transform()) {
      drawVertex(painter, vertex);
    }
  }
}

void ProjectionPane::drawAxis(QPainter &painter) {
  for (const auto &[vec, color] :
       {std::make_tuple(QVector3D{1, 0, 0}, Qt::red),
        std::make_tuple(QVector3D{0, 1, 0}, Qt::green),
        std::make_tuple(QVector3D{0, 0, 1}, Qt::blue)}) {
    drawVertex(painter, {.a = QVector3D{0, 0, 0}, .b = vec, .color = color});
  };
}

void ProjectionPane::drawVertex(QPainter &painter,
                                const Model::Vertex &vertex) {
  auto pen = QPen{};
  pen.setWidth(1);
  pen.setColor(vertex.color);
  painter.setPen(pen);

  painter.drawLine(
      vertex.a.project(modelViewMatrix, projectionMatrix, rect()).toPoint(),
      vertex.b.project(modelViewMatrix, projectionMatrix, rect()).toPoint());
}
