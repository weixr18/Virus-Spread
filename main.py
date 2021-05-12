import random
import numpy as np
import sys
import os.path

from PyQt5.QtCore import QPoint, Qt, pyqtSignal, QThread
from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.uic import loadUi

from visual.utils import load_data, load_dll
from visual.show_area import ShowArea
from visual.plot import plot


class SimulationThread(QThread):
    finish_signal = pyqtSignal()

    def __init__(self):
        super(QThread, self).__init__()
        self.dll = load_dll("./bin/sim.dll")

    def my_start(self, HOSPITAL_CAPACITY, P_MOVE,
                 P_INFECT, P_VACCINATION, P_PROTECTION, P_OBSERVE):
        self.HOSPITAL_CAPACITY = HOSPITAL_CAPACITY
        self.P_MOVE = P_MOVE
        self.P_INFECT = P_INFECT
        self.P_VACCINATION = P_VACCINATION
        self.P_PROTECTION = P_PROTECTION
        self.P_OBSERVE = P_OBSERVE
        self.start()

    def run(self):
        self.dll.sim(30, self.HOSPITAL_CAPACITY, self.P_MOVE, self.P_INFECT,
                     self.P_VACCINATION, self.P_PROTECTION, self.P_OBSERVE)
        self.finish_signal.emit()
        pass


class MainWidget(QWidget):
    def __init__(self) -> None:
        QWidget.__init__(self)
        loadUi("visual/main.ui", self)
        self.setWindowTitle('Virus Spread')

        self.sim_thread = SimulationThread()
        self.sim_thread.finish_signal.connect(self.on_simulation_complete)
        self.day_slider.sliderMoved.connect(self.on_slider_moved)
        self.run_btn.clicked.connect(self.run)
        self.counts = None

        self.HOSPITAL_CAPACITY = 100
        self.P_MOVE = 0.9
        self.P_INFECT = 0.6
        self.P_VACCINATION = 0.4
        self.P_PROTECTION = 0.8
        self.P_OBSERVE = 0.0
        self.value1.setValue(self.HOSPITAL_CAPACITY)
        self.value2.setValue(self.P_MOVE)
        self.value3.setValue(self.P_INFECT)
        self.value4.setValue(self.P_VACCINATION)
        self.value5.setValue(self.P_PROTECTION)
        self.value6.setValue(self.P_OBSERVE)

        self.file_name = self.get_file_name()
        print(self.file_name)
        if not os.path.isfile('./data/'+self.file_name):
            self.gb_param.setEnabled(False)
            self.gb_run.setEnabled(False)
            self.gb_result.setEnabled(False)
            self.sim_thread.my_start(self.HOSPITAL_CAPACITY, self.P_MOVE,
                                     self.P_INFECT, self.P_VACCINATION,
                                     self.P_PROTECTION, self.P_OBSERVE)
        else:
            self.load_data(show_day=0)

        self.show()

    def load_data(self, show_day=0):
        data, self.counts = load_data(self.file_name)
        self.showArea.setData(data)
        self.on_slider_moved(show_day)
        pass

    def run(self):
        self.HOSPITAL_CAPACITY = self.value1.value()
        self.P_MOVE = self.value2.value()
        self.P_INFECT = self.value3.value()
        self.P_VACCINATION = self.value4.value()
        self.P_PROTECTION = self.value5.value()
        self.P_OBSERVE = self.value6.value()
        self.file_name = self.get_file_name()
        print(self.file_name)
        if not os.path.isfile('./data/'+self.file_name):
            self.gb_param.setEnabled(False)
            self.gb_run.setEnabled(False)
            self.gb_result.setEnabled(False)
            self.sim_thread.my_start(self.HOSPITAL_CAPACITY, self.P_MOVE,
                                     self.P_INFECT, self.P_VACCINATION,
                                     self.P_PROTECTION, self.P_OBSERVE)
        else:
            print("File exist.")
            show_day = self.day_slider.value()
            self.load_data(show_day)

    def on_simulation_complete(self):
        self.gb_param.setEnabled(True)
        self.gb_run.setEnabled(True)
        self.gb_result.setEnabled(True)
        show_day = self.day_slider.value()
        self.load_data(show_day)
        pass

    def on_slider_moved(self, day):
        self.showArea.draw_data(day)
        self.day_label.setText(str(day))
        if self.counts is not None:
            self.le_healthy.setText(str(self.counts[0, day]))
            self.le_infected.setText(str(self.counts[1, day]))
            self.le_confirmed.setText(str(self.counts[2, day]))
            self.le_hospitalized.setText(str(self.counts[3, day]))
            self.le_healed.setText(str(self.counts[4, day]))
            self.le_dead.setText(str(self.counts[5, day]))
            self.le_observed.setText(str(self.counts[6, day]))
        pass

    def get_file_name(self):
        str_ = "C{0:0>3d}_M{1:.2f}_I{2:.2f}_V{3:.2f}_P{4:.2f}_O{5:.2f}.txt".format(
            self.HOSPITAL_CAPACITY,
            self.P_MOVE,
            self.P_INFECT,
            self.P_VACCINATION,
            self.P_PROTECTION,
            self.P_OBSERVE,
        )
        return str_


if __name__ == '__main__':
    if sys.argv.__len__() > 1 and sys.argv[1] == 'p':
        plot()
    else:
        app = QApplication(sys.argv)
        w = MainWidget()
        sys.exit(app.exec_())
    pass
