#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

struct TransformationSlider : QWidget {
  QSlider *slider = new QSlider(Qt::Orientation::Horizontal);
  explicit TransformationSlider(const QString &);
};
