#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include "Video/overlay.h"
#include "Project/project.h"
#include "DrawingItems/frameitem.h"
#include "DrawingItems/rectitem.h"
#include "DrawingItems/circleitem.h"


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

public slots:
    void set_overlay(Overlay* overlay);
    void set_video_project(VideoProject* vid_proj);
    void add_drawing(Shapes* shape, int frame_nr);
    void tree_item_clicked(QTreeWidgetItem* item, const int& col = 0);
    void context_menu(const QPoint& point);
    void rename_item();
    void remove_item();

signals:
    void jump_to_frame(VideoProject*, int);
    void set_current_drawing(Shapes* shape);
    void delete_drawing(Shapes* shape);
    void clear_frame(int);
};

#endif // DRAWINGWIDGET_H