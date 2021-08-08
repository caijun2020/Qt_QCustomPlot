#-------------------------------------------------
#
# Project created by QtCreator 2021-08-02T19:06:00
#
#-------------------------------------------------

QT       += core gui \
    widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

TARGET = WaveformViewer
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    WaveformViewer.cpp \
    qcustomplot.cpp

HEADERS  += MainWindow.h \
    WaveformViewer.h \
    qcustomplot.h

FORMS    += MainWindow.ui
