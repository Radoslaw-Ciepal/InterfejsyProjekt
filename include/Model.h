#include <QColor>
#include <QMatrix4x4>
#include <QVector3D>
#include <optional>

struct Model {
  struct Vertex {
    QVector3D a, b;
    QColor color;
  };

  QVector3D rotation{0, 0, 0}, scale{1, 1, 1}, translation{0, 0, 0};
  [[nodiscard]] std::vector<Vertex> transform() const;
  static std::optional<Model> fromGeoFile(const std::string &);
  std::vector<Vertex> vertices;
};
