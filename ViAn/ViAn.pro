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
SOURCES += main.cpp


#
# TEST
#
SOURCES += Test/test_video_player.cpp\
    Test/filehandlertest.cpp\
    Test/test_mainwindow.cpp
HEADERS += Test/test_video_player.h \
    Test/filehandlertest.h \
    Test/test_mainwindow.h


#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/icononbuttonhandler.cpp \
    GUI/inputwindow.cpp \
    GUI/qtreeitems.cpp \
    GUI/customdialog.cpp


HEADERS  += GUI/mainwindow.h \
    GUI/icononbuttonhandler.h \
    GUI/inputwindow.h \
    GUI/action.h \
    GUI/qtreeitems.h \
    GUI/customdialog.h


FORMS    += GUI/mainwindow.ui \
   GUI/inputwindow.ui

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += Video/video_player.cpp \
    Video/overlay.cpp \
    Video/shapes/arrow.cpp \
    Video/shapes/circle.cpp \
    Video/shapes/line.cpp \
    Video/shapes/pen.cpp \
    Video/shapes/rectangle.cpp \
    Video/shapes/shape.cpp \
    Video/shapes/text.cpp \
    Video/shapes/zoomrectangle.cpp
HEADERS += Video/video_player.h \
    Video/overlay.h \
    Video/shapes/arrow.h \
    Video/shapes/circle.h \
    Video/shapes/line.h \
    Video/shapes/pen.h \
    Video/shapes/rectangle.h \
    Video/shapes/shape.h \
    Video/shapes/text.h \
    Video/shapes/zoomrectangle.h
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
    Filehandler/project.cpp \
    Filehandler/analysis.cpp \
    Filehandler/video.cpp

HEADERS  += Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/dir.h \
    Filehandler/analysis.h\
    Filehandler/video.h

win32{

    SOURCES += Filehandler/stringhelper.cpp\
      Filehandler/win32dir.cpp


    HEADERS += Filehandler/stringhelper.h
}

macx {
    SOURCES += Filehandler/macdir.cpp
}

linux {
    #SOURCES += Filehandler/linuxdir.cpp
}
unix {
    SOURCES += Filehandler/linuxdir.cpp
}

#
# END: FILEHANDLER
#
