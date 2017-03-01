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

SOURCES += main.cpp
#
# GUI
#
#SOURCES +=
#    GUI/mainwindow.cpp \
#    GUI/icononbuttonhandler.cpp \
#    Video/video_player.cpp \
#    Test/test_video_player.cpp \
#    Test/filehandlertest.cpp

#HEADERS  += GUI/mainwindow.h \
#    GUI/icononbuttonhandler.h \
#    Video/video_player.h \
#    Test/test_video_player.h \
#    Test/filehandlertest.h

FORMS    += GUI/mainwindow.ui

RESOURCES += \
    resources.qrc
#
# VIDEOPLAYER
# OPENCV
#
#win32 {
#    INCLUDEPATH += C:\opencv\release\install\include
#    LIBS += C:\opencv\release\bin\libopencv_core320.dll
#    LIBS += C:\opencv\release\bin\libopencv_imgproc320.dll
#    LIBS += C:\opencv\release\bin\libopencv_imgcodecs320.dll
#    LIBS += C:\opencv\release\bin\libopencv_video320.dll
#    LIBS += C:\opencv\release\bin\libopencv_videoio320.dll
#    LIBS += C:\opencv\release\bin\libopencv_highgui320.dll
#}

#unix {
#    INCLUDEPATH += -L/usr/include/opencv2
#    LIBS += -lopencv_core
#    LIBS += -lopencv_imgproc
#    LIBS += -lopencv_imgcodecs
#    LIBS += -lopencv_video
#    LIBS += -lopencv_videoio
#    LIBS += -lopencv_highgui
#}


#
# FILEHANDLER
#
SOURCES +=\
    Filehandler/filehandler.cpp \
    Filehandler/project.cpp \


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
    Filehandler/dir.h


