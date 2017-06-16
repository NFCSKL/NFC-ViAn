#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <Filehandler/filehandler.h>

class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    ProjectManager* m_project_manager;

    QTreeWidgetItem* m_videos;
public:
    explicit ProjectWidget(ProjectManager *project_manager, QWidget *parent = nullptr);
    Project* m_proj = nullptr;

signals:
    void selected_media();
public slots:
    void new_project(void);
    void add_project(QString project_name, QString project_path);
    void add_video();
    void add_analysis();
private slots:
    void tree_item_clicked(QTreeWidgetItem *item, int col);
private:
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, QString video_name);
    void tree_add_analysis();

};

#endif // PROJECTWIDGET_H
