#ifndef VIDEOEDITLIST_H
#define VIDEOEDITLIST_H

#include "Project/video.h"

#include <QListWidget>

class Project;
class VideoProject;
class VideoEditList : public QListWidget {
    Q_OBJECT

    QListWidgetItem* clicked_item = nullptr;
    Project* m_proj = nullptr;

public:
    VideoEditList();

private:
    void item_right_clicked(const QPoint pos);
    void get_video_info(std::vector<QSize>* sizes, std::vector<int>* fpses, QSize* max_size);
    void load_intervals();

    bool horizontalLayout;

private slots:
    void context_menu(const QPoint& point);
    void edit_item(QListWidgetItem *item);
    void remove_item(QListWidgetItem* item);
    void toggle_viewlayout();
    void show_video();
public slots:
    void generate_video();
    void add_interval(int start, int end, VideoProject *vid_proj);
    void set_project(Project* proj);
    void save_item_data();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void set_video(VideoProject*, VideoState state);
    void add_video(QString path);
};

#endif // VIDEOEDITLIST_H
