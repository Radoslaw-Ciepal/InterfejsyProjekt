#include <AngleDial.h>

AngleDial::AngleDial(const QString &labelText) {
  setLayout(new QVBoxLayout{});

  dial->setMinimum(-180);
  dial->setMaximum(180);
  dial->setWrapping(true);

  auto *angleValue = new QLabel(valueText());
  QObject::connect(dial, &QDial::valueChanged, [this, angleValue](int) {
    angleValue->setText(valueText());
  });

  auto *labelsContainer = new QWidget();
  auto *labelsContainerLayout = new QHBoxLayout();
  labelsContainer->setLayout(labelsContainerLayout);
  labelsContainerLayout->addWidget(new QLabel(labelText + ":"));
  labelsContainerLayout->addStretch();
  labelsContainerLayout->addWidget(angleValue);
  layout()->addWidget(dial);
  layout()->addWidget(labelsContainer);
}
