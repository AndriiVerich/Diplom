from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys

app = QtWidgets.QApplication([])
ui = uic.loadUi("design.ui")
ui.setWindowTitle("Диплом")

serial = QSerialPort()
serial.setBaudRate(19200)

portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
ui.comL.addItems(portList)

listX_1 = [x for x in range(100)]
listY_1 = [0]*100
listY_2 = [0]*100

def onRead():
    rx = serial.readLine()
    # print(rx)
    rxs = str(rx, 'utf-8').strip()
    data = rxs.split(',')
    if data[0] == '0':
        global listY_1
        listY_1 = listY_1[1:]
        try:
            listY_1.append(int(data[1]))

        except:
            listY_1.append(listY_1[-1])
        ui.graph_1.clear()
        ui.graph_1.plot(listX_1, listY_1)
    if data[0] == '1':
        global listY_2
        listY_2 = listY_2[1:]
        try:
            listY_2.append(int(data[1]))

        except:
            listY_2.append(listY_2[-1])
        ui.graph_2.clear()
        ui.graph_2.plot(listX_1, listY_2)


def onOpen():
    serial.setPortName((ui.comL.currentText()))
    serial.open(QIODevice.ReadWrite)


def onClose():
    serial.close()

def sendSet():
    txs = ui.textF.displayText()
    serial.write(txs.encode())


serial.readyRead.connect(onRead)
ui.openB.clicked.connect(onOpen)
ui.closeB.clicked.connect(onClose)
ui.sendB.clicked.connect(sendSet)


ui.show()
app.exec()