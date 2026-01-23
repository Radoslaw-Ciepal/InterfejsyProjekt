#ifndef TRANSFORMATION_SLIDER_HPP
#define TRANSFORMATION_SLIDER_HPP

#include <QLabel>
#include <QSlider>

class TransformationSlider : public QWidget {
  Q_OBJECT
  QLabel *m_titleLabel;
  QSlider *m_slider;
  QLabel *m_valueLabel;

private slots:
  void updateLabel() const;

signals:
  void valueChanged(int);

public:
  explicit TransformationSlider(const QString &, QWidget *);
  explicit TransformationSlider(const QString &);
  [[nodiscard]] int value() const;
  void setValue(int);
};

#endif
