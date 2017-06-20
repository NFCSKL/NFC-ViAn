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
SOURCES += main.cpp \
    Project/Analysis/analysismeta.cpp \
    GUI/TreeItems/itemtypes.cpp \
    Project/projecttestsuite.cpp \
    GUI/Toolbars/maintoolbar.cpp \
    GUI/Toolbars/drawingtoolbar.cpp \
    Video/videoplayer.cpp

#
# TEST
#
SOURCES += Test/test_video_player.cpp\
    Test/test_report_generator.cpp

HEADERS += Test/test_video_player.h \
    Test/test_report_generator.h \
    Project/Analysis/analysismeta.h \
    GUI/TreeItems/itemtypes.h \
    Project/projecttestsuite.h \
    GUI/Toolbars/maintoolbar.h \
    GUI/Toolbars/drawingtoolbar.h \
    Video/videoplayer.h

#
# LIBRARY
#
SOURCES += Library/customdialog.cpp
HEADERS += Library/customdialog.h

#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/qtreeitems.cpp \
    GUI/bookmarkview.cpp \
    GUI/bookmarkitem.cpp \
    GUI/reportgenerator.cpp \
    GUI/fpsdialog.cpp \
    GUI/videowidget.cpp \
    GUI/framewidget.cpp \
    GUI/projectwidget.cpp \
    GUI/bookmarkwidget.cpp \
    GUI/createprojectdialog.cpp


HEADERS  += GUI/mainwindow.h \
    GUI/action.h \
    GUI/qtreeitems.h \
    GUI/reportgenerator.h \
    GUI/bookmarkview.h \
    GUI/bookmarkitem.h \
    GUI/fpsdialog.h \
    GUI/videowidget.h \
    GUI/framewidget.h \
    GUI/projectwidget.h \
    GUI/bookmarkwidget.h \
    GUI/createprojectdialog.h

FORMS    +=

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
SOURCES += \
    Filehandler/saveable.cpp \



HEADERS  += \
    Filehandler/saveable.h \



#
# END: FILEHANDLER
#

#
# Project
#
SOURCES += Project/report.cpp \
    Project/project.cpp \
    Project/video.cpp \
    Project/videoproject.cpp \
    Project/bookmark.cpp \
    Project/Analysis/analysis.cpp \
    Project/Analysis/ooi.cpp \
    Project/Analysis/poi.cpp


HEADERS += Project/project.h \
    Project/video.h \
    Project/videoproject.h \
    Project/bookmark.h  \
    Project/report.h \
    Project/Analysis/analysis.h \
    Project/Analysis/ooi.h \
    Project/Analysis/poi.h




#
# ANALYSIS
#
SOURCES += Analysis/AnalysisMethod.cpp\
    Analysis/MotionDetection.cpp \
    Analysis/AnalysisController.cpp

HEADERS +=Analysis/AnalysisMethod.h\
    Analysis/MotionDetection.h \
    Analysis/AnalysisController.h
