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


SOURCES += main.cpp \
    GUI/mainwindow.cpp \
    GUI/icononbuttonhandler.cpp \
    Video/video_player.cpp \
    Test/test_video_player.cpp

HEADERS  += GUI/mainwindow.h \
    GUI/icononbuttonhandler.h \
    Video/video_player.h \
    Test/test_video_player.h

FORMS    += GUI/mainwindow.ui

RESOURCES += \
    resources.qrc

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
