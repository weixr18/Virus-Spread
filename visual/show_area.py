import sys
import random
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

PEN_SIZE = 5
green_pen = QPen(Qt.green, PEN_SIZE, Qt.SolidLine)  # healthy
yellow_pen = QPen(QColor(0xff, 0xaa, 0x33), PEN_SIZE, Qt.SolidLine)  # infected
red_pen = QPen(Qt.red, PEN_SIZE, Qt.SolidLine)  # confirmed
orange_pen = QPen(QColor(0xee, 0x77, 0x00), PEN_SIZE,
                  Qt.SolidLine)  # hospitalized
blue_pen = QPen(Qt.blue, PEN_SIZE, Qt.SolidLine)  # healed
black_pen = QPen(Qt.black, PEN_SIZE, Qt.SolidLine)  # dead
pens = {
    0: green_pen,
    1: yellow_pen,
    2: red_pen,
    3: orange_pen,
    4: blue_pen,
    5: black_pen,
}


class ShowArea(QWidget):
    def __init__(self, parent=None):
        super(QWidget, self).__init__(parent)
        self.pix = QPixmap(800, 700)
        self.pix.fill(Qt.white)

    def setData(self, data):
        self.data = data

    def draw_data(self, day: int):
        data = self.data[day]
        width = self.pix.size().width()
        height = self.pix.size().height()
        pp = QPainter(self.pix)
        pp.eraseRect(0, 0, width, height)

        pp.setPen(black_pen)
        pp.drawPoint(800, 700)
        for i in range(5000):
            if(data[i][2]+data[i][3] < 0):
                continue
            pp.setPen(pens[data[i][1]])
            pp.drawPoint(data[i][3], data[i][2])
        self.update()
        pass

    def paintEvent(self, a0):
        qPainter = QPainter(self)
        qPainter.drawPixmap(0, 0, self.pix)
        qPainter.end()
        pass
