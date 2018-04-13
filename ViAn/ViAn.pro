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
    reportgenerator.cpp \
    GUI/Toolbars/maintoolbar.cpp \
    GUI/Toolbars/drawingtoolbar.cpp \
    utility.cpp \
    GUI/drawscrollarea.cpp \
    Video/zoomer.cpp \
    GUI/Analysis/analysisslider.cpp \
    Project/Test/videoprojecttest.cpp \
    GUI/Bookmark/bookmarkcategory.cpp \
    imagegenerator.cpp \
    GUI/Bookmark/bookmarklist.cpp \
    GUI/statusbar.cpp \
    GUI/Analysis/analysiswidget.cpp \
    GUI/manipulatordialog.cpp \
    Video/frameprocessor.cpp \
    Project/projecttreestate.cpp \
    GUI/Analysis/tagdialog.cpp \
    imageexporter.cpp \
    Project/Analysis/tag.cpp \
    Video/videocontroller.cpp \
    Video/videoplayer.cpp \
    Project/recentproject.cpp \
    Project/Analysis/basicanalysis.cpp \
    Project/Analysis/analysisinterval.cpp \
    Filehandler/writeable.cpp \
    Project/Analysis/analysisproxy.cpp \
    Project/Analysis/detectionbox.cpp \
    GUI/doubleclickbutton.cpp \
    GUI/TreeItems/videoitem.cpp \
    GUI/TreeItems/treeitem.cpp \
    GUI/recentprojectdialog.cpp \
    GUI/projectwidget.cpp \
    GUI/projectdialog.cpp \
    GUI/TreeItems/folderitem.cpp \
    GUI/TreeItems/tagitem.cpp \
    GUI/TreeItems/analysisitem.cpp \
    GUI/fpsdialog.cpp \
    GUI/frameexporterdialog.cpp \
    GUI/framewidget.cpp \
    GUI/videowidget.cpp \
    Analysis/analysissettings.cpp \
    GUI/Analysis/analysisdialog.cpp \
    GUI/Analysis/videolistitem.cpp \
    GUI/Analysis/queuewidget.cpp \
    GUI/DrawingItems/frameitem.cpp \
    GUI/DrawingItems/rectitem.cpp \
    GUI/drawingwidget.cpp \
    Video/shapes/shapes.cpp \
    GUI/DrawingItems/circleitem.cpp \
    GUI/DrawingItems/lineitem.cpp \
    GUI/DrawingItems/arrowitem.cpp \
    GUI/DrawingItems/shapeitem.cpp \
    GUI/DrawingItems/textitem.cpp \
    GUI/DrawingItems/penitem.cpp \
    Project/Analysis/drawingtag.cpp \
    GUI/TreeItems/drawingtagitem.cpp \
    Project/Test/bookmarktest.cpp

#
# TEST
#
SOURCES +=

HEADERS += reportgenerator.h\
    GUI/Toolbars/maintoolbar.h \
    GUI/Toolbars/drawingtoolbar.h \
    utility.h \
    GUI/drawscrollarea.h \
    Video/zoomer.h \
    GUI/Analysis/analysisslider.h \
    Project/Test/videoprojecttest.h \
    GUI/Bookmark/bookmarkcategory.h \
    imagegenerator.h \
    GUI/Bookmark/bookmarklist.h \
    GUI/statusbar.h \
    GUI/Analysis/analysiswidget.h \
    GUI/manipulatordialog.h \
    Video/frameprocessor.h \
    Project/projecttreestate.h \
    GUI/Analysis/tagdialog.h \
    imageexporter.h \
    Project/Analysis/tag.h \
    Video/videocontroller.h \
    Video/videoplayer.h \
    Project/recentproject.h \
    Project/Analysis/basicanalysis.h \
    Project/Analysis/analysisinterval.h \
    Filehandler/writeable.h \
    Project/Analysis/analysisproxy.h \
    Project/Analysis/detectionbox.h \
    GUI/doubleclickbutton.h \
    GUI/TreeItems/tagitem.h \
    GUI/TreeItems/videoitem.h \
    GUI/TreeItems/analysisitem.h \
    GUI/TreeItems/treeitem.h \
    GUI/projectdialog.h \
    GUI/projectwidget.h \
    GUI/recentprojectdialog.h \
    GUI/fpsdialog.h \
    GUI/frameexporterdialog.h \
    GUI/framewidget.h \
    GUI/videowidget.h \
    Analysis/analysissettings.h \
    Analysis/analysismethod.h \
    GUI/Analysis/analysisdialog.h \
    GUI/Analysis/videolistitem.h \
    GUI/Analysis/queuewidget.h \
    GUI/DrawingItems/frameitem.h \
    GUI/DrawingItems/rectitem.h \
    GUI/drawingwidget.h \
    Video/shapes/shapes.h \
    GUI/DrawingItems/circleitem.h \
    GUI/DrawingItems/lineitem.h \
    GUI/DrawingItems/arrowitem.h \
    GUI/DrawingItems/shapeitem.h \
    GUI/DrawingItems/textitem.h \
    GUI/DrawingItems/penitem.h \
    Project/Analysis/drawingtag.h \
    GUI/TreeItems/drawingtagitem.h \
    Project/Test/bookmarktest.h

#
# LIBRARY
#
SOURCES += Library/customdialog.cpp
HEADERS += Library/customdialog.h

#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/Bookmark/bookmarkitem.cpp \
    GUI/Bookmark/bookmarkwidget.cpp


HEADERS  += GUI/mainwindow.h \
    GUI/Bookmark/bookmarkitem.h \
    GUI/Bookmark/bookmarkwidget.h

FORMS    +=

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += \
    Video/overlay.cpp \
    Video/shapes/arrow.cpp \
    Video/shapes/circle.cpp \
    Video/shapes/line.cpp \
    Video/shapes/pen.cpp \
    Video/shapes/rectangle.cpp \
    Video/shapes/text.cpp \
    Video/shapes/zoomrectangle.cpp \
    Video/shapes/analysarea.cpp \
    Video/framemanipulator.cpp
HEADERS += \
    Video/overlay.h \
    Video/shapes/arrow.h \
    Video/shapes/circle.h \
    Video/shapes/line.h \
    Video/shapes/pen.h \
    Video/shapes/rectangle.h \
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
SOURCES += Analysis/analysismethod.cpp\
    Analysis/motiondetection.cpp

HEADERS += Analysis/motiondetection.h
