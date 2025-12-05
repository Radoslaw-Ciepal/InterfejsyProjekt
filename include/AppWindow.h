#include <ProjectionPane.h>
#include <QMainWindow>

struct AppWindow : QMainWindow {
  ProjectionPane *projectionPane = new ProjectionPane();

  explicit AppWindow(std::shared_ptr<Model> model);
  void resizeEvent(QResizeEvent *event) override;
};
