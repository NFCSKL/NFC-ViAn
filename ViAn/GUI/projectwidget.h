#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include "Analysis/analysissettings.h"
#include "Project/video.h"

#include <QAction>
#include <QPointer>
#include <QStandardPaths>
#include <QTreeWidget>

#include <stack>

class AnalysisItem;
class AnalysisMethod;
class AnalysisProxy;
class BasicAnalysis;
class Interval;
class IntervalItem;
class Project;
class Tag;
class TagFrame;
class TagItem;
class TreeItem;
class VideoItem;
class VideoProject;
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* selection_parent = nullptr;
    std::set<QString> allowed_vid_exts {"mkv", "flv", "vob", "ogv", "ogg",
                                "264", "263", "mjpeg", "avc", "m2ts",
                                "mts", "avi", "mov", "qt", "wmv", "mp4",
                                "m4p", "m4v", "mpg", "mp2", "mpeg",
                                "mpe", "mpv", "m2v", "m4v", "3gp", "3g2",
                                "flv", "f4v", "f4p", "f4a", "f4b", "webm"};
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    ~ProjectWidget();
    Project* m_proj = nullptr;
    TagItem* m_tag_item = nullptr;
    IntervalItem* m_interval_item = nullptr;
    AnalysisSettings* analysis_settings = new AnalysisSettings();
    QPointer<QAction> show_details_act = nullptr;
    QPointer<QAction> show_settings_act = nullptr;

    std::vector<Video*> video_list;
    std::vector<QString> remove_list;

signals:
    void selected_media();
    void marked_video_state(VideoProject *vid_proj, VideoState state);
    void set_project(Project*);

    void marked_analysis(AnalysisProxy*);
    void marked_basic_analysis(BasicAnalysis*);
    void toggle_analysis_details();
    void toggle_settings_details();
    void update_settings_wgt(AnalysisSettings*);
    void show_analysis_settings(bool);

    void set_zoom_tool();
    void set_show_analysis_details(bool);
    void set_detections(bool);
    void enable_poi_btns(bool, bool);
    void set_poi_slider(bool);
    void set_tag_slider(bool);
    void set_interval_slider(bool);
    void set_video_project(VideoProject*);
    void clear_tag();
    void clear_slider();
    void set_status_bar(QString);
    void begin_analysis(QTreeWidgetItem*, AnalysisMethod*);
    void update_slider();
    void remove_overlay();
    void new_vid_proj(VideoProject*);
    void item_type(int);
    void update_tag();

public slots:
    void new_project(void);
    void add_project(const QString project_name, const QString project_path);
    void add_video();
    void create_video(QString path);
    void add_images();
    void create_sequence(QStringList image_paths, QStringList checksums, QString path);
    void start_analysis(VideoProject*, AnalysisSettings*settings = nullptr);
    void add_tag(VideoProject*, Tag *tag);
    void add_frames_to_tag_item(TreeItem *item);
    void add_new_frame_to_tag_item(int frame, TagFrame *t_frame);
    void remove_frame_from_tag_item(int frame);
    void add_interval(VideoProject*, Interval* interval);
    void add_interval_area_to_interval(TreeItem* item, int current = -1);
    void set_interval_area(int new_area_start);
    void set_tree_item_name(QTreeWidgetItem *item, QString);
    void toggle_details(bool b);
    void toggle_settings(bool b);
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
    void remove_sequence_item(QTreeWidgetItem* item);
    void remove_interval_item(QTreeWidgetItem* item);
    void remove_interval_area_item(QTreeWidgetItem* item);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void update_analysis_settings();
    void advanced_analysis();
    bool prompt_save();
    void update_current_videoitem(QString path);
    void update_videoitems();
private slots:
    void context_menu(const QPoint& point);
    void open_video_in_widget();
    void remove_item();
    void rename_item();
    void drawing_tag();
    void update_settings();
    void create_folder_item();
    void tree_item_changed(QTreeWidgetItem *item, QTreeWidgetItem *prev_item = nullptr);
    void check_selection();
    void check_selection_level(QTreeWidgetItem* current, QTreeWidgetItem* prev);

    void update_item_data(QTreeWidgetItem *item, int column);
private:
    bool is_analysis_running(QTreeWidgetItem* root = nullptr);
    void set_main_window_name(QString name);
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    QStringList mimeTypes() const;
    void file_dropped(QString path);
    void folder_dropped(QString path);
    std::stack<int> get_index_path(QTreeWidgetItem* item);
    VideoItem* get_video_item(VideoProject* v_proj, QTreeWidgetItem* s_item = nullptr);
    void get_video_items(QTreeWidgetItem* root, std::vector<VideoItem *> &items);
    void get_analysis_items(QTreeWidgetItem* root, std::vector<AnalysisItem*> &items);
    void insert_to_path_index(VideoProject* vid_proj);
    void save_item_data(QTreeWidgetItem* item = nullptr);
    void add_analyses_to_item(VideoItem* v_item);
    void update_current_tag(VideoItem* v_item);
    void update_current_interval(VideoItem* v_item);
    bool message_box(QString text = "", QString info_text = "", bool warning = false);
    std::vector<VideoProject*> removed_sequences;
signals:
    void close_all_widgets();
    void project_closed();
    void item_removed(VideoProject* vid_proj);
    void open_in_widget(VideoProject* vid_proj);
    void save_draw_wgt(QTreeWidgetItem* = nullptr);
    void save_bmark_wgt();
    void clear_analysis();
    void abort_all_analysis();
};

#endif // PROJECTWIDGET_H
