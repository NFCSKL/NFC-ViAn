#ifndef VIDEOEDITLIST_H
#define VIDEOEDITLIST_H

#include <QListWidget>

class VideoProject;
class VideoEditList : public QListWidget {
    Q_OBJECT

    QListWidgetItem* clicked_item = nullptr;
    QString m_proj_path = "";

public:
    VideoEditList();

private:
    void item_right_clicked(const QPoint pos);
    void get_video_info(std::vector<QSize>* sizes, std::vector<int>* fpses, QSize* max_size);

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

};

#endif // VIDEOEDITLIST_H
