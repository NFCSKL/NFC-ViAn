#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <Filehandler/filehandler.h>

class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    FileHandler* m_file_handler;
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(QString vid_name);

    QTreeWidgetItem* videos;
public:
    explicit ProjectWidget(FileHandler* file_handler, QWidget *parent = nullptr);
    Project* proj = nullptr;

signals:
    void selected_media();
public slots:
    void new_project(void);
    void add_project(QString project_name, QString project_path);
    void add_video();
};

#endif // PROJECTWIDGET_H
