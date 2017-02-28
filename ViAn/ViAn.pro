#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T11:51:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViAn
TEMPLATE = app



SOURCES += main.cpp \
    GUI/mainwindow.cpp \
    Filehandler/filehandler.cpp \
    Filehandler/project.cpp \
    Filehandler/filehandler_testsuite.cpp \


win32{

SOURCES += Filehandler/stringhelper.cpp\
    Filehandler/win32dir.cpp


HEADERS += Filehandler/stringhelper.h
}

macx {
SOURCES += Filehandler/macdir.cpp
}
linux {
    SOURCES += Filehandler/linuxdir.cpp\
    Filehandler/macdir.cpp
}


HEADERS  += mainwindow.h\
    Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/dir.h \
    Filehandler/filehandler_testsuite.h




FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11

QT += testlib

