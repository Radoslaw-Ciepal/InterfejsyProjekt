#include "MainWindow.hpp"
#include "Model.hpp"
#include "docks/ProjectionDock.hpp"
#include "docks/TransformationDock.hpp"
#include <QFileDialog>
#include <QMenuBar>
#include <QVector3D>
#include <filesystem>
#include <fstream>
#include <memory>

MainWindow::MainWindow() : m_model(std::make_shared<Model>()) {
  setWindowTitle("Rzutowanie brył");
  resize(800, 600);

  auto *fileMenu = menuBar()->addMenu("Plik");
  auto *loadModelAction = fileMenu->addAction(
      "Wczytaj model...",
      QKeyCombination(Qt::KeyboardModifier::ControlModifier, Qt::Key::Key_O),
      this, &MainWindow::openModel);
  auto *newViewAction = fileMenu->addAction(
      "Nowy widok",
      QKeyCombination(Qt::KeyboardModifier::ControlModifier, Qt::Key::Key_N),
      this, &MainWindow::newView);

  auto *transformationDock = new TransformationDock(this);
  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, transformationDock);
  connect(transformationDock, &TransformationDock::valueChanged, this,
          &MainWindow::setTransformationMatrix);
  setTransformationMatrix(transformationDock->transformation());

  newView();
  newView();
  newView();
}

bool MainWindow::openModel() {
  std::filesystem::path modelPath =
      QFileDialog::getOpenFileName(this, "Wczytaj model...", "", "*.geo")
          .toStdString();

  if (!std::filesystem::is_regular_file(modelPath)) {
    return false;
  }

  auto file = std::ifstream(modelPath);
  if (!file.is_open()) {
    return false;
  }

  m_model = std::make_shared<Model>(load_geo(file));

  emit modelChanged(m_model);
  return true;
}

void MainWindow::setTransformationMatrix(QMatrix4x4 transformationMatrix) {
  m_transformationMatrix = std::make_shared<QMatrix4x4>(transformationMatrix);
  emit transformationMatrixChanged(m_transformationMatrix);
}

void MainWindow::newView() {
  auto *projectionDock = new ProjectionDock(this);
  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, projectionDock);
  connect(this, &MainWindow::modelChanged, projectionDock,
          &ProjectionDock::setModel);
  connect(this, &MainWindow::transformationMatrixChanged, projectionDock,
          &ProjectionDock::setTransformationMatrix);

  projectionDock->setTransformationMatrix(m_transformationMatrix);
  projectionDock->setModel(m_model);
};
