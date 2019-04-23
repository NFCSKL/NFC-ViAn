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
SOURCES += \
    Analysis/analysismethod.cpp \
    Analysis/analysissettings.cpp \
    Analysis/motiondetection.cpp \
    Filehandler/saveable.cpp \
    Filehandler/writeable.cpp \
    GUI/Analysis/analysisdialog.cpp \
    GUI/Analysis/analysislistitem.cpp \
    GUI/Analysis/analysisslider.cpp \
    GUI/Analysis/analysiswidget.cpp \
    GUI/Analysis/anasettingwidget.cpp \
    GUI/Analysis/queuewidget.cpp \
    GUI/Analysis/tagdialog.cpp \
    GUI/Analysis/videolistitem.cpp \
    GUI/Bookmark/bookmarkcategory.cpp \
    GUI/Bookmark/bookmarkdialog.cpp \
    GUI/Bookmark/bookmarkitem.cpp \
    GUI/Bookmark/bookmarklist.cpp \
    GUI/Bookmark/bookmarkwidget.cpp \
    GUI/DrawingItems/arrowitem.cpp \
    GUI/DrawingItems/circleitem.cpp \
    GUI/DrawingItems/frameitem.cpp \
    GUI/DrawingItems/lineitem.cpp \
    GUI/DrawingItems/penitem.cpp \
    GUI/DrawingItems/rectitem.cpp \
    GUI/DrawingItems/shapeitem.cpp \
    GUI/DrawingItems/textitem.cpp \
    GUI/Toolbars/drawingtoolbar.cpp \
    GUI/Toolbars/maintoolbar.cpp \
    GUI/TreeItems/analysisitem.cpp \
    GUI/TreeItems/drawingtagitem.cpp \
    GUI/TreeItems/folderitem.cpp \
    GUI/TreeItems/intervalareaitem.cpp \
    GUI/TreeItems/intervalitem.cpp \
    GUI/TreeItems/sequencecontaineritem.cpp \
    GUI/TreeItems/sequenceitem.cpp \
    GUI/TreeItems/tagframeitem.cpp \
    GUI/TreeItems/tagitem.cpp \
    GUI/TreeItems/treeitem.cpp \
    GUI/TreeItems/videoitem.cpp \
    GUI/YoloWidget/yolowidget.cpp \
    GUI/YoloWidget/yolowidgetitem.cpp \
    GUI/doubleclickbutton.cpp \
    GUI/drawingwidget.cpp \
    GUI/drawscrollarea.cpp \
    GUI/frameexporterdialog.cpp \
    GUI/framewidget.cpp \
    GUI/mainwindow.cpp \
    GUI/manipulatorwidget.cpp \
    GUI/pathdialog.cpp \
    GUI/projectdialog.cpp \
    GUI/projectwidget.cpp \
    GUI/recentprojectdialog.cpp \
    GUI/sequencedialog.cpp \
    GUI/settingsdialog.cpp \
    GUI/statusbar.cpp \
    GUI/textdialog.cpp \
    GUI/videowidget.cpp \
    GUI/viewpathdialog.cpp \
    GUI/viewpathitem.cpp \
    GUI/zoompreviewwidget.cpp \
    Project/Analysis/analysis.cpp \
    Project/Analysis/analysisinterval.cpp \
    Project/Analysis/analysisproxy.cpp \
    Project/Analysis/basicanalysis.cpp \
    Project/Analysis/detectionbox.cpp \
    Project/Analysis/interval.cpp \
    Project/Analysis/poi.cpp \
    Project/Analysis/tag.cpp \
    Project/Analysis/tagframe.cpp \
    Project/Test/bookmarktest.cpp \
    Project/Test/projecttestsuite.cpp \
    Project/Test/videoprojecttest.cpp \
    Project/bookmark.cpp \
    Project/imagesequence.cpp \
    Project/project.cpp \
    Project/projecttreestate.cpp \
    Project/recentproject.cpp \
    Project/report.cpp \
    Project/video.cpp \
    Project/videoproject.cpp \
    Test/test_video_player.cpp \
    Video/shapes/arrow.cpp \
    Video/shapes/circle.cpp \
    Video/shapes/line.cpp \
    Video/shapes/pen.cpp \
    Video/shapes/rectangle.cpp \
    Video/shapes/shapes.cpp \
    Video/shapes/text.cpp \
    Video/framemanipulator.cpp \
    Video/frameprocessor.cpp \
    Video/overlay.cpp \
    Video/videocontroller.cpp \
    Video/videoplayer.cpp \
    Video/zoomer.cpp \
    constants.cpp \
    imageexporter.cpp \
    imagegenerator.cpp \
    imageimporter.cpp \
    main.cpp \
    reportgenerator.cpp \
    utility.cpp

HEADERS += \
    Analysis/analysismethod.h \
    Analysis/analysissettings.h \
    Analysis/motiondetection.h \
    Filehandler/saveable.h \
    Filehandler/writeable.h \
    GUI/Analysis/analysisdialog.h \
    GUI/Analysis/analysislistitem.h \
    GUI/Analysis/analysisslider.h \
    GUI/Analysis/analysiswidget.h \
    GUI/Analysis/anasettingwidget.h \
    GUI/Analysis/queuewidget.h \
    GUI/Analysis/tagdialog.h \
    GUI/Analysis/videolistitem.h \
    GUI/Bookmark/bookmarkcategory.h \
    GUI/Bookmark/bookmarkdialog.h \
    GUI/Bookmark/bookmarkitem.h \
    GUI/Bookmark/bookmarklist.h \
    GUI/Bookmark/bookmarkwidget.h \
    GUI/DrawingItems/arrowitem.h \
    GUI/DrawingItems/circleitem.h \
    GUI/DrawingItems/frameitem.h \
    GUI/DrawingItems/lineitem.h \
    GUI/DrawingItems/penitem.h \
    GUI/DrawingItems/rectitem.h \
    GUI/DrawingItems/shapeitem.h \
    GUI/DrawingItems/textitem.h \
    GUI/Toolbars/drawingtoolbar.h \
    GUI/Toolbars/maintoolbar.h \
    GUI/TreeItems/analysisitem.h \
    GUI/TreeItems/drawingtagitem.h \
    GUI/TreeItems/folderitem.h \
    GUI/TreeItems/intervalareaitem.h \
    GUI/TreeItems/intervalitem.h \
    GUI/TreeItems/sequencecontaineritem.h \
    GUI/TreeItems/sequenceitem.h \
    GUI/TreeItems/tagframeitem.h \
    GUI/TreeItems/tagitem.h \
    GUI/TreeItems/treeitem.h \
    GUI/TreeItems/videoitem.h \
    GUI/YoloWidget/yolowidget.h \
    GUI/YoloWidget/yolowidgetitem.h \
    GUI/doubleclickbutton.h \
    GUI/drawingwidget.h \
    GUI/drawscrollarea.h \
    GUI/frameexporterdialog.h \
    GUI/framewidget.h \
    GUI/mainwindow.h \
    GUI/manipulatorwidget.h \
    GUI/pathdialog.h \
    GUI/projectdialog.h \
    GUI/projectwidget.h \
    GUI/recentprojectdialog.h \
    GUI/sequencedialog.h \
    GUI/settingsdialog.h \
    GUI/statusbar.h \
    GUI/textdialog.h \
    GUI/videowidget.h \
    GUI/viewpathdialog.h \
    GUI/viewpathitem.h \
    GUI/zoompreviewwidget.h \
    Project/Analysis/analysis.h \
    Project/Analysis/analysisinterval.h \
    Project/Analysis/analysisproxy.h \
    Project/Analysis/basicanalysis.h \
    Project/Analysis/detectionbox.h \
    Project/Analysis/interval.h \
    Project/Analysis/poi.h \
    Project/Analysis/tag.h \
    Project/Analysis/tagframe.h \
    Project/Test/bookmarktest.h \
    Project/Test/projecttestsuite.h \
    Project/Test/videoprojecttest.h \
    Project/bookmark.h \
    Project/imagesequence.h \
    Project/project.h \
    Project/projecttreestate.h \
    Project/recentproject.h \
    Project/report.h \
    Project/video.h \
    Project/videoproject.h \
    Test/test_video_player.h \
    Video/shapes/arrow.h \
    Video/shapes/circle.h \
    Video/shapes/line.h \
    Video/shapes/pen.h \
    Video/shapes/rectangle.h \
    Video/shapes/shapes.h \
    Video/shapes/text.h \
    Video/framemanipulator.h \
    Video/frameprocessor.h \
    Video/overlay.h \
    Video/videocontroller.h \
    Video/videoplayer.h \
    Video/zoomer.h \
    constants.h \
    imageexporter.h \
    imagegenerator.h \
    imageimporter.h \
    reportgenerator.h \
    table.h \
    utility.h

FORMS    +=

RESOURCES += resources.qrc

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
