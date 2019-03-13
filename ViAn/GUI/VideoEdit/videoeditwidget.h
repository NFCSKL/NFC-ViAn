#ifndef VIDEOEDITWIDGET_H
#define VIDEOEDITWIDGET_H

#include "GUI/VideoEdit/videoeditlist.h"

#include <QWidget>

class QScrollArea;
class QVBoxLayout;
class VideoProject;

class VideoEditWidget : public QWidget
{
    Q_OBJECT
    QScrollArea*    scroll_area;
    QVBoxLayout*    layout;
    VideoEditList*  videoedit_list;

public:
    explicit VideoEditWidget(QWidget *parent = nullptr);

signals:
    void interval_to_edit(int, int, VideoProject*);

public slots:
};

#endif // VIDEOEDITWIDGET_H
