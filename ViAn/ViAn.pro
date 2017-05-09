#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T11:51:07
#
#-------------------------------------------------

QT       += core gui
QT       += testlib
QT       += axcontainer

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
    Test/test_mainwindow.cpp \
    Test/test_report_generator.cpp \
    Test/overlayintegrationtest.cpp
HEADERS += Test/test_video_player.h \
    Test/filehandlertest.h \
    Test/test_mainwindow.h \
    Test/test_report_generator.h \
    Test/overlayintegrationtest.h

#
# LIBRARY
#
SOURCES += Library/customdialog.cpp
HEADERS += Library/customdialog.h

#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/icononbuttonhandler.cpp \
    GUI/qtreeitems.cpp \
    GUI/bookmarkview.cpp \
    GUI/bookmarkitem.cpp \
    GUI/reportgenerator.cpp \
    GUI/makeproject.cpp \
    GUI/fpsdialog.cpp



HEADERS  += GUI/mainwindow.h \
    GUI/icononbuttonhandler.h \
    GUI/action.h \
    GUI/qtreeitems.h \
    GUI/reportgenerator.h \
    GUI/bookmarkview.h \
    GUI/bookmarkitem.h \
    GUI/makeproject.h \
    GUI/fpsdialog.h

FORMS    += GUI/mainwindow.ui \
    GUI/makeproject.ui

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += Video/video_player.cpp \
    Video/overlay.cpp \
    Video/analysisoverlay.cpp \
    Video/shapes/arrow.cpp \
    Video/shapes/circle.cpp \
    Video/shapes/line.cpp \
    Video/shapes/pen.cpp \
    Video/shapes/rectangle.cpp \
    Video/shapes/shape.cpp \
    Video/shapes/text.cpp \
    Video/shapes/zoomrectangle.cpp \
    Video/shapes/analysarea.cpp
HEADERS += Video/video_player.h \
    Video/overlay.h \
    Video/analysisoverlay.h \
    Video/shapes/arrow.h \
    Video/shapes/circle.h \
    Video/shapes/line.h \
    Video/shapes/pen.h \
    Video/shapes/rectangle.h \
    Video/shapes/shape.h \
    Video/shapes/text.h \
    Video/shapes/zoomrectangle.h \
    Video/shapes/analysarea.h
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
    Filehandler/video.cpp \
    Filehandler/videoproject.cpp \
    Filehandler/bookmark.cpp \
    Filehandler/saveable.cpp \
    Filehandler/report.cpp


HEADERS  += Filehandler/filehandler.h \
    Filehandler/project.h \
    Filehandler/dir.h \
    Filehandler/video.h \
    Filehandler/videoproject.h \
    Filehandler/bookmark.h  \
    Filehandler/saveable.h \
    Filehandler/report.h

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
