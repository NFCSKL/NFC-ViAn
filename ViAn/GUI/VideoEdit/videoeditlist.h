#ifndef VIDEOEDITLIST_H
#define VIDEOEDITLIST_H

#include <QListWidget>

class VideoProject;
class VideoEditList : public QListWidget {
    Q_OBJECT

    QListWidgetItem* clicked_item = nullptr;

public:
    VideoEditList();

private:
    void item_right_clicked(const QPoint pos);

    bool horizontalLayout;

private slots:
    void context_menu(const QPoint& point);
    void edit_item(QListWidgetItem *item);
    void remove_item();
    void toggle_viewlayout();
    void show_video();
public slots:
    void generate_video();
    void add_interval(int start, int end, VideoProject *vid_proj);
protected:
//    void mousePressEvent(QMouseEvent* event) override;

};

#endif // VIDEOEDITLIST_H
