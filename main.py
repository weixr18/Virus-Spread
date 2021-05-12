import random
import numpy as np
import sys
import os.path

from PyQt5.QtCore import QPoint, Qt
from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.uic import loadUi

from visual.data import load_data
from visual.show_area import ShowArea
from visual.load_dll import load_dll


class MainWidget(QWidget):
    def __init__(self) -> None:
        QWidget.__init__(self)
        loadUi("visual/main.ui", self)
        self.setWindowTitle('Virus Spread')

        self.file_name = "C500_M0.50_I0.10_V0.90_P0.50.txt"
        self.load_data(show_day=0)
        self.day_slider.sliderMoved.connect(self.on_slider_moved)
        self.run_btn.clicked.connect(self.run)

        self.dll = load_dll("./bin/sim.dll")
        self.HOSPITAL_CAPACITY = 100
        self.P_MOVE = 0.9
        self.P_INFECT = 0.6
        self.P_VACCINATION = 0.4
        self.P_PROTECTION = 0.8

        self.show()

    def load_data(self, show_day=0):
        data = load_data(self.file_name)
        self.showArea.setData(data)
        self.showArea.draw_data(show_day)
        pass

    def on_slider_moved(self, day):
        self.showArea.draw_data(day)
        self.day_label.setText(str(day))

    def run(self):
        self.HOSPITAL_CAPACITY = self.value1.value()
        self.P_MOVE = self.value2.value()
        self.P_INFECT = self.value3.value()
        self.P_VACCINATION = self.value4.value()
        self.P_PROTECTION = self.value5.value()
        self.file_name = self.get_file_name()
        print(self.file_name)
        if not os.path.isfile('./data/'+self.file_name):
            self.dll.sim(30, self.HOSPITAL_CAPACITY, self.P_MOVE,
                         self.P_INFECT, self.P_VACCINATION, self.P_PROTECTION)
        else:
            print("File exist.")

        show_day = self.day_slider.value()
        self.load_data(show_day)

    def get_file_name(self):
        str_ = "C{0}_M{1:.2f}_I{2:.2f}_V{3:.2f}_P{4:.2f}.txt".format(
            self.HOSPITAL_CAPACITY,
            round(self.P_MOVE, 2),
            round(self.P_INFECT, 2),
            round(self.P_VACCINATION, 2),
            round(self.P_PROTECTION, 2)
        )
        return str_


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWidget()
    sys.exit(app.exec_())
    pass
