# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'c:\Dev\project-chess\frontend\App-Chess\chessview.ui'
#
# Created by: PyQt5 UI code generator 5.15.10
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_ChessView(object):
    def setupUi(self, ChessView):
        ChessView.setObjectName("ChessView")
        ChessView.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(ChessView)
        self.centralwidget.setObjectName("centralwidget")
        ChessView.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(ChessView)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName("menubar")
        ChessView.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(ChessView)
        self.statusbar.setObjectName("statusbar")
        ChessView.setStatusBar(self.statusbar)

        self.retranslateUi(ChessView)
        QtCore.QMetaObject.connectSlotsByName(ChessView)

    def retranslateUi(self, ChessView):
        _translate = QtCore.QCoreApplication.translate
        ChessView.setWindowTitle(_translate("ChessView", "ChessView"))
