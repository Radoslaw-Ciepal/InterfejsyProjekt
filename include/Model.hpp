#ifndef MODEL_HPP
#define MODEL_HPP

#include <QColor>
#include <QVector3D>
#include <fstream>

struct Edge {
  QVector3D a, b;
  QColor color;
};

using Model = std::vector<Edge>;
using ModelView = std::span<const Edge>;
template <std::size_t N> using StaticModel = std::array<Edge, N>;

Model load_geo(std::ifstream &);

inline constexpr StaticModel axis_model = {
    Edge{.a = {0, 0, 0}, .b = {1, 0, 0}, .color = {255, 0, 0}},
    Edge{.a = {0, 0, 0}, .b = {0, 1, 0}, .color = {0, 255, 0}},
    Edge{.a = {0, 0, 0}, .b = {0, 0, 1}, .color = {0, 0, 255}}};

#endif
