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
    void set_video(VideoProject*, VideoState state);
    void add_video(QString path);
    void set_project(Project* proj);
    void save_item_data();

public slots:
    void clear_intervals();
    void generate_video();
};

#endif // VIDEOEDITWIDGET_H
