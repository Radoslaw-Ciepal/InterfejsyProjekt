#ifndef PROJECTION_MODE_HPP
#define PROJECTION_MODE_HPP

#include <array>
#include <cstddef>
#include <cstdint>

enum class ProjectionMode : uint8_t {
  Perspective,
  Ortho,
  Oblique,
  Isometric,
  Count,
};

extern const std::array<const char16_t *, std::size_t(ProjectionMode::Count)> ModeLabels;

#endif
