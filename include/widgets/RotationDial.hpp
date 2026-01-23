#ifndef ROTATION_DIAL_HPP
#define ROTATION_DIAL_HPP

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlayout.h>

class RotationDial : public QWidget {
  Q_OBJECT

  QDial *m_dial;
  QLabel *m_label;
  QString m_title;

private slots:
  void updateLabel(int);

public:
  explicit RotationDial(QString, QWidget *);
  explicit RotationDial(QString);
  [[nodiscard]] int value() const;
  void setValue(int);

signals:
  void valueChanged(int);
};

#endif
