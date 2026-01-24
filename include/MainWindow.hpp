#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "Model.hpp"
#include <QMainWindow>
#include <QMatrix4x4>
#include <memory>

class MainWindow : public QMainWindow {
  Q_OBJECT
  std::shared_ptr<Model> m_model;
  std::shared_ptr<QMatrix4x4> m_transformationMatrix;
  bool openModel();

public slots:
	void setTransformationMatrix(QMatrix4x4);

signals:
	void modelChanged(std::shared_ptr<Model>);
	void transformationMatrixChanged(std::shared_ptr<QMatrix4x4>);

public:
  explicit MainWindow();
  void newView();
};

#endif
