#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T21:11:44
#
#-------------------------------------------------

QT       += core gui sql

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += c:/_dev/_boost/boost_1_55_0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = duhpaycalc
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp

HEADERS  += \
    mainwindow.h

FORMS    += \
    mainwindow.ui
