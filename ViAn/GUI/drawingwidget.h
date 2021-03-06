#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include "Project/video.h"

#include <QTreeWidget>

class FrameItem;
class Overlay;
class Shapes;
class VideoProject;

class DrawingWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DrawingWidget(QWidget *parent = nullptr);
    Overlay* m_overlay = nullptr;
    VideoProject* m_vid_proj = nullptr;

    void update_from_overlay();

private:
    void add_item_in_order(FrameItem *item);
    void remove_from_tree(QTreeWidgetItem* item);
    void add_drawings_to_frame(FrameItem *f_item);
public slots:
    void clear_overlay();
    void set_overlay(Overlay* overlay);
    void set_video_project(VideoProject* vid_proj);
    void add_drawing(Shapes* shape, int frame_nr);
    void save_item_data(QTreeWidgetItem* item = nullptr);
    void set_current_selected(Shapes* shape, int frame_nr);
    void tree_item_clicked(QTreeWidgetItem* item, const int& col = 0);
    void context_menu(const QPoint& point);
    void rename_item();
    void remove_item();
    void remove_item_frame(int frame);
    void delete_item();
    void item_changed(QTreeWidgetItem*);

signals:
    void jump_to_frame(VideoProject*, VideoState);
    void set_current_drawing(Shapes* shape);
    void set_tool_edit();
    void set_tool_zoom();
    void delete_drawing(Shapes* shape);
    void clear_frame(int);
    void update_text(QString, Shapes*);
};

#endif // DRAWINGWIDGET_H
