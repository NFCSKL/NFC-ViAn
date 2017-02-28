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
    Filehandler/testsuite_filehandler.cpp \

win32{

SOURCES += Filehandler/stringhelper.cpp\
    Filehandler/win32dir.cpp


HEADERS += Filehandler/stringhelper.h\
    Filehandler/win32dir.h
}

macx|linux {
SOURCES +=     Filehandler/linux_mac_dir.cpp

HEADERS += Filehandler/unixdir.h
}


HEADERS  += mainwindow.h\
    Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/testsuite_filehandler.h \
    Filehandler/dir.h




FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11



