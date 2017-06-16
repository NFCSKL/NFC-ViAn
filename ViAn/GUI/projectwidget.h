#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <Filehandler/filehandler.h>

class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    ProjectManager* m_project_manager;
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(QString vid_name);

    QTreeWidgetItem* videos;
public:
    explicit ProjectWidget(ProjectManager *project_manager, QWidget *parent = nullptr);
    Project* proj = nullptr;

signals:
    void selected_media();
public slots:
    void new_project(void);
    void add_project(QString project_name, QString project_path);
    void add_video();
};

#endif // PROJECTWIDGET_H
