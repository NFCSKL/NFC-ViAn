#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include "Project/project.h"
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* m_videos;
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    Project* m_proj = nullptr;

signals:
    void selected_media();
    void marked_video();
public slots:
    void new_project(void);
    void add_project(QString project_name, QString project_path);
    void add_video();
    void add_analysis();
    void save_project();
    void open_project();
private slots:
    void tree_item_clicked(QTreeWidgetItem *item, int col);
private:
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, QString video_name);
    void tree_add_analysis();

};

#endif // PROJECTWIDGET_H
