#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
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
    void marked_video(VideoProject* vid_proj, int frame = 0);
    void proj_path(std::string);
    void load_bookmarks(VideoProject* vid_proj);
    void marked_analysis(Analysis*);
    void marked_tag(Analysis*);
    void set_detections(bool);
    void enable_poi_btns(bool, bool);
    void set_poi_slider(bool);
    void set_tag_slider(bool);
    void set_status_bar(QString);
    void begin_analysis(std::string, std::string, QTreeWidgetItem*);

public slots:
    void new_project(void);
    void add_project(const QString project_name, const QString project_path);
    void add_video();
    void start_analysis(VideoProject*);
    void add_tag(VideoProject*, Analysis tag);
    void set_tree_item_name(QTreeWidgetItem *item, QString);
    void save_project();
    void open_project();
    void close_project();
    void remove_project();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void tree_item_clicked(QTreeWidgetItem *item, const int& col = -1);
private:
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    QStringList mimeTypes() const;
    void file_dropped(QString path);
    void folder_dropped(QString path);
signals:
    void project_closed();
protected:


};

#endif // PROJECTWIDGET_H
