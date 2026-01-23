#include "ProjectionMode.hpp"
#include <cstddef>
#include <cuchar>

namespace {
using ModeLabelsArray =
    std::array<const char16_t *, std::size_t(ProjectionMode::Count)>;

constexpr ModeLabelsArray initModeLabels() {
  ModeLabelsArray ModeLabels = {};
  ModeLabels[std::size_t(ProjectionMode::Ortho)] = u"Ortogonalny";
  ModeLabels[std::size_t(ProjectionMode::Perspective)] = u"Perspektywiczny";
  ModeLabels[std::size_t(ProjectionMode::Oblique)] = u"Ukośny";
  ModeLabels[std::size_t(ProjectionMode::Isometric)] = u"Izometryczny";
  return ModeLabels;
}
} // namespace

constexpr const ModeLabelsArray ModeLabels = initModeLabels();
