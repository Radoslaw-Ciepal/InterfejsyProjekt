#!/usr/bin/env python3

import sys
from collections import namedtuple
from itertools import batched, chain
from pathlib import Path
import math

from PySide6 import QtWidgets, QtGui, QtCore
from PySide6.QtCore import Qt


class GeoModel:
    Vertex = namedtuple("Vertex", ("a", "b", "color"))

    def __init__(self):
        self.vertices = None
        self.rotation = QtGui.QVector3D(0, 0, 0)
        self.scale = QtGui.QVector3D(1, 1, 1)
        self.translation = QtGui.QVector3D(0, 0, 0)

    def loadFromFile(self, filename: str):
        file = Path(filename)
        if not file.is_file():
            return
        self.vertices = [
            GeoModel.Vertex(
                QtGui.QVector3D(*a), QtGui.QVector3D(*b), QtGui.QColor(*color)
            )
            for a, b, color in batched(
                batched(
                    map(
                        float,
                        chain(*map(str.split, file.read_text().splitlines())),
                    ),
                    3,
                ),
                3,
            )
        ]

    def __iter__(self):
        if not self.vertices:
            self.currentVertexStream = iter([])
            return self

        self.transformMatrix = QtGui.QMatrix4x4()
        self.transformMatrix.scale(self.scale)
        self.transformMatrix.translate(self.translation)
        self.transformMatrix.rotate(self.rotation.x(), 1, 0, 0)
        self.transformMatrix.rotate(self.rotation.y(), 0, 1, 0)
        self.transformMatrix.rotate(self.rotation.z(), 0, 0, 1)

        self.currentVertexStream = iter(self.vertices)
        return self

    def __next__(self):
        a, b, color = next(self.currentVertexStream)
        return GeoModel.Vertex(
            self.transformMatrix.map(a), self.transformMatrix.map(b), color
        )


model = GeoModel()
model.loadFromFile("a10a_4.geo")

class RenderingPane(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setBackgroundRole(QtGui.QPalette.ColorRole.Base)
        self.setAutoFillBackground(True)
        self.setSizePolicy(
            QtWidgets.QSizePolicy.Policy.Expanding,
            QtWidgets.QSizePolicy.Policy.Expanding,
        )
        self.setMode(0)

    def setMode(self, mode):
        self.projectionMatrix = QtGui.QMatrix4x4()
        self.modelViewMatrix = QtGui.QMatrix4x4()
        def setOrho():
            self.projectionMatrix.ortho(-2, 2, -2, 2, -2, 2)

        if mode == 0:
            # perspektywa (1 punkt)
            self.projectionMatrix.perspective(90, self.width() / self.height(), 0.1, 100)
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(2, 2, 2),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, 0, -1),
            )
        elif mode == 1:
            # orto góra
            setOrho()
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(0, 0, 5),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, -1, 0),
            )
        elif mode == 2:
            # orto dół
            setOrho()
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(0, 0, -5),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, -1, 0),
            )
        elif mode == 3:
            # orto przód
            setOrho()
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(0, 5, 0),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, 0, -1),
            )
        elif mode == 4:
            # ukośny gabinetowy
            k = 0.5
            alpha = math.pi / 4
            self.projectionMatrix.setColumn(
                2, QtGui.QVector4D(k * math.cos(alpha), k * math.sin(alpha), 0, 0)
            )
            self.projectionMatrix.translate(0.5, 0.5, 0)
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(1, 1, 0),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, 0, -1),
            )
        elif mode == 5:
            # ukośny kawaleryjski
            k = 1
            alpha = math.pi / 4
            self.projectionMatrix.setColumn(
                2, QtGui.QVector4D(k * math.cos(alpha), k * math.sin(alpha), 0, 0)
            )
            self.projectionMatrix.translate(0.5, 0.5, 0)
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(1, 1, 0),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, 0, -1),
            )
        elif mode == 6:
            # izometryczny
            setOrho()
            self.modelViewMatrix.lookAt(
                QtGui.QVector3D(2, 2, 2 * math.sqrt(2 / 3)),
                QtGui.QVector3D(0, 0, 0),
                QtGui.QVector3D(0, 0, -1),
            )

    def paintEvent(self, _event):
        painter = QtGui.QPainter(self)
        self.drawAxis(painter)
        for vertex in model:
            self.drawVertex(painter, *vertex)

    def drawVertex(
        self,
        painter: QtGui.QPainter,
        a: QtGui.QVector3D,
        b: QtGui.QVector3D,
        color=Qt.GlobalColor.white,
    ):
        pen = QtGui.QPen()
        pen.setWidth(1)
        pen.setColor(color)
        painter.setPen(pen)

        proj_a = a.project(self.modelViewMatrix, self.projectionMatrix, self.rect())
        proj_b = b.project(self.modelViewMatrix, self.projectionMatrix, self.rect())
        painter.drawLine(proj_a.toPoint(), proj_b.toPoint())

    def drawAxis(self, painter: QtGui.QPainter):
        for vec, color in (
            (QtGui.QVector3D(1, 0, 0), Qt.GlobalColor.red),
            (QtGui.QVector3D(0, 1, 0), Qt.GlobalColor.green),
            (QtGui.QVector3D(0, 0, 1), Qt.GlobalColor.blue),
        ):
            self.drawVertex(painter, QtGui.QVector3D(0, 0, 0), vec, color)


class AngleDial(QtWidgets.QWidget):
    def __init__(self, labelText: str):
        super().__init__()
        self.setLayout(QtWidgets.QVBoxLayout())

        self.dial = QtWidgets.QDial()
        self.dial.setMinimum(-180)
        self.dial.setMaximum(180)
        self.dial.setWrapping(True)

        labelsContainer = QtWidgets.QWidget()
        labelsContainer.setLayout(QtWidgets.QHBoxLayout())

        def valueText():
            return f"{self.dial.value()}\u00b0"

        labelsContainer.layout().addWidget(QtWidgets.QLabel(labelText + ":"))
        labelsContainer.layout().addStretch()

        angleValue = QtWidgets.QLabel(valueText())
        self.dial.valueChanged.connect(lambda: angleValue.setText(valueText()))
        labelsContainer.layout().addWidget(angleValue)

        self.layout().addWidget(self.dial)
        self.layout().addWidget(labelsContainer)


class TransformationSlider(QtWidgets.QWidget):
    def __init__(self, labelText: str):
        super().__init__()
        self.setLayout(QtWidgets.QHBoxLayout())
        self.slider = QtWidgets.QSlider(Qt.Orientation.Horizontal)

        self.layout().addWidget(QtWidgets.QLabel(labelText))
        self.layout().addWidget(self.slider)

        valueLabel = QtWidgets.QLabel(str(self.slider.value()))
        self.slider.valueChanged.connect(
            lambda: valueLabel.setText(str(self.slider.value()))
        )
        self.layout().addWidget(valueLabel)


class AppWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.renderingPane = RenderingPane()
        self.setCentralWidget(self.renderingPane)

    def resizeEvent(self, _event):
        self.renderingPane.update()


if __name__ == "__main__":
    app = QtWidgets.QApplication()
    window = AppWindow()
    window.setWindowTitle("Rzutowanie brył")

    fileMenu = window.menuBar().addMenu("Plik")
    loadModelAction = fileMenu.addAction(
        "Wczytaj model...",
        lambda: model.loadFromFile(
            QtWidgets.QFileDialog.getOpenFileName(
                window, "Wczytaj model...", "", "*.geo"
            )[0]
        ),
    )
    loadModelAction.setShortcut(
        QtCore.QKeyCombination(Qt.KeyboardModifier.ControlModifier, Qt.Key.Key_O)
    )

    transformationsPanelDock = QtWidgets.QDockWidget("Przekształcenia")
    transformationsPanel = QtWidgets.QWidget()
    transformationsPanelDock.setWidget(transformationsPanel)
    transformationsPanelLayout = QtWidgets.QVBoxLayout()
    transformationsPanelLayout.setSpacing(32)
    transformationsPanelLayout.setContentsMargins(16, 32, 16, 32)
    transformationsPanel.setLayout(transformationsPanelLayout)
    window.addDockWidget(
        Qt.DockWidgetArea.RightDockWidgetArea, transformationsPanelDock
    )

    translateSlidersGroup = QtWidgets.QGroupBox("Przesunięcie")
    translateSlidersGroupLayout = QtWidgets.QVBoxLayout()
    translateSlidersGroup.setLayout(translateSlidersGroupLayout)
    transformationsPanelLayout.addWidget(translateSlidersGroup)

    vectorAxes = {"X": QtGui.QVector3D.setX, "Y": QtGui.QVector3D.setY, "Z": QtGui.QVector3D.setZ}

    for axis, setter in vectorAxes.items():
        slider = TransformationSlider(axis)
        slider.slider.setMinimum(-5)
        slider.slider.setMaximum(5)
        slider.slider.valueChanged.connect(lambda v: setter(model.translation, v))
        slider.slider.valueChanged.connect(lambda _: window.renderingPane.update())
        translateSlidersGroupLayout.addWidget(slider)

    rotationDialsGroup = QtWidgets.QGroupBox("Obrót")
    rotationDialsGroupLayout = QtWidgets.QHBoxLayout()
    rotationDialsGroup.setLayout(rotationDialsGroupLayout)
    transformationsPanelLayout.addWidget(rotationDialsGroup)

    for name, setter in vectorAxes.items():
        dial = AngleDial(name)
        dial.dial.valueChanged.connect(lambda v: setter(model.rotation, v))
        dial.dial.valueChanged.connect(lambda _: window.renderingPane.update())
        rotationDialsGroupLayout.addWidget(dial)

    scaleSlidersGroup = QtWidgets.QGroupBox("Skalowanie")
    scaleSlidersGroupLayout = QtWidgets.QVBoxLayout()
    scaleSlidersGroup.setLayout(scaleSlidersGroupLayout)
    transformationsPanelLayout.addWidget(scaleSlidersGroup)

    for name, setter in vectorAxes.items():
        slider = TransformationSlider(name)
        slider.slider.setMinimum(1)
        slider.slider.setMaximum(5)
        slider.slider.valueChanged.connect(lambda v: setter(model.scale, v))
        slider.slider.valueChanged.connect(lambda _: window.renderingPane.update())
        scaleSlidersGroupLayout.addWidget(slider)

    projectionModeDropdown = QtWidgets.QComboBox()
    transformationsPanelLayout.addWidget(projectionModeDropdown)
    projectionModeDropdown.addItems(
        [
            "Perspektywiczny (1 punkt zbieżności)",
            "Ortogonalny (Góra)",
            "Ortogonalny (Dół)",
            "Ortogonalny (Przód)",
            "Ukośny (Gabinetowy)",
            "Ukośny (Kawaleryjski)",
            "Izometryczny",
        ]
    )
    projectionModeDropdown.currentIndexChanged.connect(window.renderingPane.setMode)
    projectionModeDropdown.currentIndexChanged.connect(
        lambda _: window.renderingPane.update()
    )

    # projectionModeDropdown.setCurrentIndex(6)
    transformationsPanelLayout.addStretch()

    window.setWindowTitle = "Rzutowanie brył"
    window.resize(800, 600)
    window.show()

    sys.exit(app.exec())
