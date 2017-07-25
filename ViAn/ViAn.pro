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
    GUI/TreeItems/itemtypes.cpp \
    GUI/Toolbars/maintoolbar.cpp \
    GUI/Toolbars/drawingtoolbar.cpp \
    utility.cpp \
    GUI/drawscrollarea.cpp \
    Video/zoomer.cpp \
    GUI/analysisslider.cpp \
    GUI/projectdialog.cpp \
    Project/Test/videoprojecttest.cpp \
    GUI/bookmarkcategory.cpp \
    imagegenerator.cpp \
    GUI/bookmarklist.cpp \
    GUI/statusbar.cpp \
    GUI/analysiswidget.cpp \
    GUI/manipulatordialog.cpp \
    Video/frameprocessor.cpp \
    GUI/tagdialog.cpp \
    imageexporter.cpp \
    GUI/frameexporterdialog.cpp \
    Project/Analysis/tag.cpp \
    Video/videocontroller.cpp \
    Video/videoplayer.cpp \
    GUI/recentprojectdialog.cpp \
    Project/recentproject.cpp \
    Project/Analysis/basicanalysis.cpp \
    Project/Analysis/analysisinterval.cpp \
    Filehandler/writeable.cpp \
    Project/Analysis/analysisproxy.cpp \
    Project/Analysis/detectionbox.cpp


#
# TEST
#
SOURCES +=\
    Test/test_report_generator.cpp

HEADERS += \
    Test/test_report_generator.h \
    GUI/TreeItems/itemtypes.h \
    GUI/Toolbars/maintoolbar.h \
    GUI/Toolbars/drawingtoolbar.h \
    utility.h \
    GUI/drawscrollarea.h \
    Video/zoomer.h \
    GUI/analysisslider.h \
    GUI/projectdialog.h \
    Project/Test/videoprojecttest.h \
    GUI/bookmarkcategory.h \
    imagegenerator.h \
    GUI/bookmarklist.h \
    GUI/statusbar.h \
    GUI/analysiswidget.h \
    GUI/manipulatordialog.h \
    Video/frameprocessor.h \
    GUI/tagdialog.h \
    imageexporter.h \
    GUI/frameexporterdialog.h \
    Project/Analysis/tag.h \
    Video/videocontroller.h \
    Video/videoplayer.h \
    GUI/recentprojectdialog.h \
    Project/recentproject.h \
    Project/Analysis/basicanalysis.h \
    Project/Analysis/analysisinterval.h \
    Filehandler/writeable.h \
    Project/Analysis/analysisproxy.h \
    Project/Analysis/detectionbox.h

#
# LIBRARY
#
SOURCES += Library/customdialog.cpp
HEADERS += Library/customdialog.h

#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/bookmarkitem.cpp \
    GUI/reportgenerator.cpp \
    GUI/fpsdialog.cpp \
    GUI/videowidget.cpp \
    GUI/framewidget.cpp \
    GUI/projectwidget.cpp \
    GUI/bookmarkwidget.cpp


HEADERS  += GUI/mainwindow.h \
    GUI/action.h \
    GUI/reportgenerator.h \
    GUI/bookmarkitem.h \
    GUI/fpsdialog.h \
    GUI/videowidget.h \
    GUI/framewidget.h \
    GUI/projectwidget.h \
    GUI/bookmarkwidget.h

FORMS    +=

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += \
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
    Video/shapes/analysarea.cpp \
    Video/framemanipulator.cpp
HEADERS += \
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
    Video/shapes/analysarea.h \
    Video/framemanipulator.h

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
SOURCES += Project/Test/projecttestsuite.cpp \
    Project/report.cpp \
    Project/project.cpp \
    Project/video.cpp \
    Project/videoproject.cpp \
    Project/bookmark.cpp \
    Project/Analysis/analysis.cpp \
    Project/Analysis/poi.cpp


HEADERS +=Project/Test/projecttestsuite.h \
    Project/project.h \
    Project/video.h \
    Project/videoproject.h \
    Project/bookmark.h  \
    Project/report.h \
    Project/Analysis/analysis.h \
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
