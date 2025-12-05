#include <QDial>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

struct AngleDial : QWidget {
  QDial *dial = new QDial;
  explicit AngleDial(const QString &);
  [[nodiscard]] QString valueText() const {
    return QString::number(dial->value()).append("\u00b0");
  };
};
