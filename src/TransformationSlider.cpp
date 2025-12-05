#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <TransformationSlider.h>

TransformationSlider::TransformationSlider(const QString &labelText) {
  setLayout(new QHBoxLayout{});

  auto *const valueLabel = new QLabel(QString::number(slider->value()));
  QObject::connect(slider, &QSlider::valueChanged, [valueLabel, this]() {
    valueLabel->setText(QString::number(slider->value()));
  });

  layout()->addWidget(new QLabel{labelText});
  layout()->addWidget(slider);
  layout()->addWidget(valueLabel);
}
