#include <Model.h>
#include <QWidget>
#include <QtGui>
#include <QtMath>
#include <numbers>

enum struct Mode : uint8_t {
  PerspectiveSingle,
  PerspectiveDouble,
  OrthoUp,
  OrthoDown,
  OrthoFront,
  ObliqueCabinet,
  ObliqueCavalier,
  Isometric
};

struct ProjectionPane : QWidget {
  using Vertices = std::vector<Model::Vertex>;

  float alpha = std::numbers::pi / 4;
  float k = 0.5f;
  std::shared_ptr<Model> model;
  Mode mode = Mode::PerspectiveSingle;
  QMatrix4x4 projectionMatrix, modelViewMatrix;

  ProjectionPane();

  [[nodiscard]] float aspectRatio() const;
  void calculateModelViewMatrix();
  void calculateProjectionMatrix();
  void drawAxis(QPainter &);
  void drawVertex(QPainter &, const Model::Vertex &);
  void paintEvent(QPaintEvent *event) override;
};
