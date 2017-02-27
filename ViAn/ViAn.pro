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
    Filehandler/win32dir.cpp \
    Filehandler/testsuite_filehandler.cpp

win32{

SOURCES += Filehandler/stringhelper.cpp
}

HEADERS  += mainwindow.h\
    Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/testsuite_filehandler.h \
    Filehandler/win32dir.h

win32{

HEADERS += Filehandler/stringhelper.h
}


FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += C:\Programmering\libraries\boost\boost_1_63_0
LIBS += -LC:/Programmering/libraries/boost/boost_1_63_0/boost
Libs += -llibboost_system-vc100-mt-1_63
