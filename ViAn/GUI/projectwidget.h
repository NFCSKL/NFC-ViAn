#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "Project/project.h"
#include "GUI/TreeItems/tagitem.h"
#include <stack>
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/tag.h"
#include "Project/videoproject.h"
#include "Analysis/analysismethod.h"
#include "Analysis/motiondetection.h"
#include "Analysis/analysisdialog.h"
#include "TreeItems/analysisitem.h"
class Project;
class VideoItem;
class FolderItem;
class AnalysisProxy;
class VideoProject;
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* clicked_item = nullptr;
    QPoint* clicked_point = nullptr;
    QTreeWidgetItem* selection_parent = nullptr;
    bool selecting = false;
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    Project* m_proj = nullptr;

signals:
    void selected_media();
    void marked_video(VideoProject* vid_proj);
    void proj_path(std::string);
    void load_bookmarks(VideoProject* vid_proj);

    void marked_analysis(AnalysisProxy*);
    void marked_basic_analysis(BasicAnalysis*);

    void set_detections(bool);
    void enable_poi_btns(bool, bool);
    void enable_tag_btn(bool);
    void set_poi_slider(bool);
    void set_tag_slider(bool);
    void set_status_bar(QString);
    void begin_analysis(QTreeWidgetItem*, AnalysisMethod*);
    void update_frame();
    void remove_overlay();
    void new_vid_proj(VideoProject*);

public slots:
    void new_project(void);
    void add_project(const QString project_name, const QString project_path);
    void add_video();
    void start_analysis(VideoProject*, AnalysisSettings*settings = nullptr);
    void add_basic_analysis(VideoProject*, BasicAnalysis *tag);
    void set_tree_item_name(QTreeWidgetItem *item, QString);
    void save_project();
    void open_project(QString project_path="");
    void close_project();
    void remove_project();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void advanced_analysis();
    void advanced_analysis_setup(AnalysisMethod*method, VideoProject *vid_proj);
private slots:
    void context_menu(const QPoint& point);
    void remove_item();
    void rename_item();
    void create_folder_item();
    void tree_item_clicked(QTreeWidgetItem *item, const int& col = 0);
    void check_selection();
    void check_selection_level(QTreeWidgetItem* current, QTreeWidgetItem* prev);
private:
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    QStringList mimeTypes() const;
    void file_dropped(QString path);
    void folder_dropped(QString path);
    void insert_dropped(VideoItem* item);
    std::stack<int> get_index_path(QTreeWidgetItem* item);
    VideoItem* get_video_item(VideoProject* v_proj, QTreeWidgetItem* s_item = nullptr);
    void get_video_items(QTreeWidgetItem* root, std::vector<VideoItem *> &items);
    void insert_to_path_index(VideoProject* vid_proj);
    void save_item_data(QTreeWidgetItem* item = nullptr);
    void add_analyses_to_item(VideoItem* v_item);
signals:
    void project_closed();


};

#endif // PROJECTWIDGET_H
