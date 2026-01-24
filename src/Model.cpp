#include "Model.hpp"
#include <iostream>

namespace {
template <typename In, typename Out, typename Arg = In>
Out read3(std::istream &in) {
  In a;
  In b;
  In c;
  in >> a >> b >> c;
  return Out(Arg(a), Arg(b), Arg(c));
}
} // namespace

Model load_geo(std::ifstream &in) {
  Model model;
  while (in) {
    const auto start = read3<float, QVector3D>(in);
    const auto end = read3<float, QVector3D>(in);
    const auto color = read3<float, QColor, int>(in);

	if (in.eof()) {
	    break;
	}

    if (in.fail()) {
      model.clear();
      break;
    }

    model.emplace_back(start, end, color);
  }
  return model;
}
