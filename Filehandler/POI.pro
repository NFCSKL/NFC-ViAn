#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T14:48:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = POI
TEMPLATE = app


SOURCES +=\
    filehandler.cpp \
    project.cpp \
    main.cpp \
    stringhelper.cpp \
    testsuite_filehandler.cpp

HEADERS  += \
    filehandler.h \
    project.h \
    stringhelper.h \
    testsuite_filehandler.h

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11
