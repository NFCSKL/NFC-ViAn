#ifndef VIDEOEDITITEM_H
#define VIDEOEDITITEM_H

#include <QListWidgetItem>
#include <QDebug>

class Project;
class VideoInterval;
class VideoProject;
class VideoEditItem : public QListWidgetItem {
    QString m_path = "";

    VideoInterval* m_interval = nullptr;

    void set_name();
    void set_icon();
public:
    VideoEditItem(VideoInterval *interval, QListWidget *parent = nullptr);
    ~VideoEditItem();

    int get_index();
    int get_start();
    int get_end();
    int get_last_frame();
    QString get_path();
    VideoInterval* get_interval();

    void set_start(int new_start);
    void set_end(int new_end);

    bool operator<(const QListWidgetItem &other) const;
};

#endif // VIDEOEDITITEM_H
