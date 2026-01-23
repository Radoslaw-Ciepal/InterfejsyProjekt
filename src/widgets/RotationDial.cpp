#include "widgets/RotationDial.hpp"
#include <QtCore/qnamespace.h>

#define _deg "\u00b0"

RotationDial::RotationDial(QString title, QWidget *parent)
    : QWidget(parent), m_title(std::move(title)), m_dial(new QDial(this)),
      m_label(new QLabel(this)) {
  m_dial->setMinimum(-180);
  m_dial->setMaximum(180);
  m_dial->setWrapping(true);

  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(m_dial);
  mainLayout->addWidget(m_label);
  mainLayout->setAlignment(m_label, Qt::AlignHCenter);

  connect(m_dial, &QDial::valueChanged, this, &RotationDial::updateLabel);
  connect(m_dial, &QDial::valueChanged, this, &RotationDial::valueChanged);

  updateLabel(m_dial->value());
}

void RotationDial::updateLabel(int value) {
    m_label->setText(QString("%1: %2" _deg).arg(m_title).arg(value));
}

void RotationDial::setValue(int value) {
    m_dial->setValue(value);
}

int RotationDial::value() const {
    return m_dial->value();
}
