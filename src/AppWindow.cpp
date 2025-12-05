#include <AppWindow.h>

AppWindow::AppWindow(std::shared_ptr<Model> model) { projectionPane->model = std::move(model); setCentralWidget(projectionPane); }
void AppWindow::resizeEvent(QResizeEvent *) { projectionPane->update(); }
