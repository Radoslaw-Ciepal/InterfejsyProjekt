#include "widgets/TransformationSlider.hpp"
#include <QHBoxLayout>

TransformationSlider::TransformationSlider(const QString &title,
                                           QWidget *parent)
    : m_titleLabel(new QLabel(title, this)),
      m_slider(new QSlider(Qt::Orientation::Horizontal, this)),
      m_valueLabel(new QLabel(this)), QWidget(parent) {
  m_slider->setMinimum(-5);
  m_slider->setMaximum(5);

  auto *mainLayout = new QHBoxLayout(this);
  mainLayout->addWidget(m_titleLabel);
  mainLayout->addWidget(m_slider);
  mainLayout->addWidget(m_valueLabel);

  connect(m_slider, &QSlider::valueChanged, this,
          &TransformationSlider::updateLabel);
  connect(m_slider, &QSlider::valueChanged, this,
          &TransformationSlider::valueChanged);

  updateLabel();
};

TransformationSlider::TransformationSlider(const QString &title)
    : TransformationSlider(title, nullptr) {};

void TransformationSlider::updateLabel() const {
  m_valueLabel->setText(QString::number(m_slider->value()));
}

[[nodiscard]] int TransformationSlider::value() const {
  return m_slider->value();
}

void TransformationSlider::setValue(int value) { m_slider->setValue(value); }
