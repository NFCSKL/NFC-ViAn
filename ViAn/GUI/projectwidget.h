#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QAction>
#include <vector>
#include "Project/project.h"
#include "GUI/TreeItems/tagitem.h"
#include "GUI/TreeItems/analysisitem.h"
#include "GUI/TreeItems/drawingtagitem.h"
#include "GUI/TreeItems/tagframeitem.h"
#include <stack>
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/tag.h"
#include "Project/Analysis/drawingtag.h"
#include "Project/videoproject.h"
#include "Analysis/analysismethod.h"
#include "Analysis/motiondetection.h"
#include "Analysis/analysisdialog.h"
#include "Analysis/anasettingwidget.h"

class Project;
class VideoItem;
class FolderItem;
class AnalysisProxy;
class VideoProject;
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* selection_parent = nullptr;
    std::set<std::string> allowed_vid_exts {"mkv", "flv", "vob", "ogv", "ogg",
                                "264", "263", "mjpeg", "avc", "m2ts",
                                "mts", "avi", "mov", "qt", "wmv", "mp4",
                                "m4p", "m4v", "mpg", "mp2", "mpeg",
                                "mpe", "mpv", "m2v", "m4v", "3gp", "3g2",
                                "flv", "f4v", "f4p", "f4a", "f4b"};
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();
    Project* m_proj = nullptr;
    TagItem* m_tag_item = nullptr;
    AnalysisSettings* analysis_settings = new AnalysisSettings();
    QPointer<QAction> show_details_act = nullptr;
    QPointer<QAction> show_settings_act = nullptr;

signals:
    void selected_media();
    void marked_video_state(VideoProject *vid_proj, VideoState state);
    void proj_path(std::string);
    void load_bookmarks(VideoProject* vid_proj);

    void marked_analysis(AnalysisProxy*);
    void marked_basic_analysis(BasicAnalysis*);
    void toggle_analysis_details();
    void toggle_settings_details();
    void update_settings_wgt(AnalysisSettings*);
    void show_analysis_settings(bool);

    void set_detections(bool);
    void enable_poi_btns(bool, bool);
    void set_poi_slider(bool);
    void set_tag_slider(bool);
    void set_video_project(VideoProject*);
    void clear_tag();
    void clear_slider();
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
    void add_tag(VideoProject*, Tag *tag);
    void add_frames_to_tag_item(TreeItem *item);
    void add_new_frame_to_tag_item(int frame, TagFrame *t_frame);
    void remove_frame_from_tag_item(int frame);
    void set_tree_item_name(QTreeWidgetItem *item, QString);
    bool save_project();
    bool open_project(QString project_path="");
    bool close_project();
    void remove_project();
    void remove_tree_item(QTreeWidgetItem* item);
    void remove_video_item(QTreeWidgetItem* item);
    void remove_tag_item(QTreeWidgetItem* item);
    void remove_drawing_tag_item(QTreeWidgetItem* item);
    void remove_analysis_item(QTreeWidgetItem* item);
    void remove_tag_frame_item(QTreeWidgetItem* item);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void update_analysis_settings();
    void advanced_analysis();
    bool prompt_save();
private slots:
    void context_menu(const QPoint& point);
    void open_video_in_widget();
    void remove_item();
    void rename_item();
    void drawing_tag();
    void toggle_details(bool b);
    void toggle_settings(bool b);
    void update_settings();
    void create_folder_item();
    void tree_item_clicked(QTreeWidgetItem *item, const int& col = 0);
    void check_selection();
    void check_selection_level(QTreeWidgetItem* current, QTreeWidgetItem* prev);

    void update_item_data(QTreeWidgetItem *item, int column);
private:
    void set_main_window_name(QString name);
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    QStringList mimeTypes() const;
    void file_dropped(QString path);
    void folder_dropped(QString path);
    std::stack<int> get_index_path(QTreeWidgetItem* item);
    VideoItem* get_video_item(VideoProject* v_proj, QTreeWidgetItem* s_item = nullptr);
    void get_video_items(QTreeWidgetItem* root, std::vector<VideoItem *> &items);
    void insert_to_path_index(VideoProject* vid_proj);
    void save_item_data(QTreeWidgetItem* item = nullptr);
    void add_analyses_to_item(VideoItem* v_item);
    void update_current_tag(VideoItem* v_item);
    bool message_box(QString text = "", QString info_text = "", bool warning = false);
signals:
    void project_closed();
    void item_removed(VideoProject* vid_proj);
    void open_in_widget(VideoProject* vid_proj);
    void save_draw_wgt(QTreeWidgetItem* = nullptr);
    void clear_analysis();
};

#endif // PROJECTWIDGET_H
