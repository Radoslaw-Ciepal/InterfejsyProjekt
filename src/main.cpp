#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  // auto model = std::make_shared<Model>();

  MainWindow window;
  window.show();

  return QApplication::exec();
}
