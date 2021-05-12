import random
import numpy as np
import sys

from PyQt5.QtCore import QPoint, Qt
from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.uic import loadUi

from visual.data import load_data
from visual.show_area import ShowArea


def ShowString():
    print("Hello EveryOne")


class MainWidget(QWidget):
    def __init__(self) -> None:
        QWidget.__init__(self)
        loadUi("visual/main.ui", self)
        self.setWindowTitle('Virus Spread')

        self.file_name = "C500_M0.50_I0.10_V0.90_P0.50.txt"
        self.load_data()
        self.day_slider.sliderMoved.connect(self.showArea.draw_data)
        self.day_slider.sliderMoved.connect(self.change_show)

        self.show()

    def load_data(self):
        data = load_data(self.file_name)
        self.showArea.setData(data)
        pass

    def change_show(self, day):
        self.day_label.setText(str(day))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWidget()
    sys.exit(app.exec_())
    pass
