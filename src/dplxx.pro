#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T20:35:10
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = dplxx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    directorywatcher.cpp

HEADERS  += mainwindow.h \
    directorywatcher.h

FORMS    += mainwindow.ui
