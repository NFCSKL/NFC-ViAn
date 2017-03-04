#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T11:51:07
#
#-------------------------------------------------

QT       += core gui
QT       += testlib

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViAn
TEMPLATE = app
#
# GENERAL
#
SOURCES += main.cpp \


#
# TEST
#
SOURCES += Test/test_video_player.cpp \
    Test/filehandlertest.cpp
HEADERS += Test/test_video_player.h \
    Test/filehandlertest.h \


#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/icononbuttonhandler.cpp \
    GUI/inputwindow.cpp



HEADERS  += GUI/mainwindow.h \
    GUI/icononbuttonhandler.h \
    GUI/inputwindow.h



FORMS    += GUI/mainwindow.ui \
   GUI/inputwindow.ui

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += Video/video_player.cpp
HEADERS += Video/video_player.h
win32 {
    INCLUDEPATH += C:\opencv\release\install\include
    LIBS += C:\opencv\release\bin\libopencv_core320.dll
    LIBS += C:\opencv\release\bin\libopencv_imgproc320.dll
    LIBS += C:\opencv\release\bin\libopencv_imgcodecs320.dll
    LIBS += C:\opencv\release\bin\libopencv_video320.dll
    LIBS += C:\opencv\release\bin\libopencv_videoio320.dll
    LIBS += C:\opencv\release\bin\libopencv_highgui320.dll
}

unix {
    INCLUDEPATH += -L/usr/include/opencv2
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_imgcodecs
    LIBS += -lopencv_video
    LIBS += -lopencv_videoio
    LIBS += -lopencv_highgui
}

#
# START: FILEHANDLER
#
SOURCES += Filehandler/filehandler.cpp \
    Filehandler/project.cpp

HEADERS  += Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/dir.h

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

#
# END: FILEHANDLER
#
