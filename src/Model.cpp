#include <Model.h>
#include <QDebug>
#include <filesystem>
#include <fstream>
#include <iostream>

std::optional<Model> Model::fromGeoFile(const std::string &source) {
  // TODO: przecież to są edges
  std::vector<Vertex> vertices;
  std::filesystem::path path(source);
  if (!std::filesystem::is_regular_file(path)) {
    return std::nullopt;
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    return std::nullopt;
  }

  float a;
  float b;
  float c;

  while (file) {
    file >> a >> b >> c;
    const QVector3D v0(a, b, c);

    file >> a >> b >> c;
    const QVector3D v1(a, b, c);

    file >> a >> b >> c;
    const QColor color(static_cast<int>(a), static_cast<int>(b),
                       static_cast<int>(c));

    vertices.emplace_back(v0, v1, color);
  }
  return Model{.vertices = vertices};
}

[[nodiscard]] std::vector<Model::Vertex> Model::transform() const {
  QMatrix4x4 transformMatrix;
  transformMatrix.scale(scale);
  transformMatrix.rotate(rotation.x(), 1, 0, 0);
  transformMatrix.rotate(rotation.y(), 0, 1, 0);
  transformMatrix.rotate(rotation.z(), 0, 0, 1);
  transformMatrix.translate(translation);

  std::vector<Vertex> transformedVertices(vertices.size());
  std::ranges::transform(vertices, transformedVertices.begin(),
                         [&transformMatrix](const Vertex &x) {
                           return Vertex{.a = transformMatrix.map(x.a),
                                         .b = transformMatrix.map(x.b),
                                         .color = x.color};
                         });
  return transformedVertices;
}
