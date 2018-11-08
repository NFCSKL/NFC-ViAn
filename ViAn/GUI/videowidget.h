#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "Project/video.h"
#include "Video/videoplayer.h"
#include "Video/shapes/shapes.h"
#include "Video/framemanipulator.h"
#include "Video/frameprocessor.h"

#include "opencv2/core/core.hpp"

#include <QTime>
#include <QWidget>

// TODO Might not need
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

class AnalysisSettings;
class AnalysisSlider;
class BasicAnalysis;
class DoubleClickButton;
class DrawScrollArea;
class FrameWidget;
class Interval;
class QCheckBox;
class QColor;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QScrollBar;
class QShortcut;
class QSlider;
class QVBoxLayout;
class Tag;
class TagFrame;
class VideoController;
class VideoProject;

class VideoWidget : public QWidget
{
    Q_OBJECT
private:
    QScrollBar* v_bar;
    QScrollBar* h_bar;
    QSize current_frame_size;
    QTime timer;

    int proj_tree_item = 1001;      // Default is VIDEO_ITEM, based on ITEM_TYPE on treeitem
    int prev_frame_idx;
    double m_scale_factor = 1;

    zoomer_settings z_settings;
    manipulation_settings m_settings;
    overlay_settings o_settings;
    video_sync v_sync;

    std::string m_video_path;

    std::atomic<int> frame_index{0};            // Shared frame index. Updated by the video player and the GUI
    std::atomic_int video_width{0};
    std::atomic_int video_height{0};
    std::atomic_int m_speed_step{0};            // Playback speed

    std::atomic_bool is_playing{false};         // True when the video player is playing
    std::atomic_bool settings_changed{false};   // True when the user changed something. Zoom, brightness etc.
    std::atomic_bool overlay_changed{false};    // True when the user have drawn something
    std::atomic_bool new_frame{false};          // True when a new frame has been loaded by the video player
    std::atomic_bool new_video{false};          // True when a new video is loaded
    std::atomic_bool new_frame_video{false};    // True when a new video has been loaded by video player but not by frameprocesser
    std::atomic_bool video_loaded{false};       // True when a video is loaded/open
    std::atomic_bool m_abort_playback{false};     // Flag used to abort playback when closing program
    std::atomic_bool m_abort_processor{false};

    std::condition_variable player_con;         // Used to notify the video player when to load a new video or when to play the current one
    std::mutex player_lock;

    int m_video_width = 0;
    int m_video_height = 0;
    int m_frame_rate = 0;
    int m_frame_length = 0;

    std::pair<int, int> m_interval = std::make_pair(-1, -1);

    FrameProcessor* f_processor;
    QThread* processing_thread;
public:
    explicit VideoWidget(QWidget *parent = nullptr, bool floating = false);
    ~VideoWidget() override;

    QVBoxLayout* vertical_layout;

    // Lock and wait condition to sleep player when video is paused
    FrameWidget* frame_wgt;
    AnalysisSlider* playback_slider;

    VideoProject* get_current_video_project();
    std::pair<int, int> get_frame_interval();
    VideoController* v_controller;

    int get_current_video_length();
    void set_overlay(Overlay* overlay);
    void set_update_text(QString, Shapes*);
    void set_clear_drawings(int frame);
    void set_delete_drawing(Shapes* shape);

signals:
    void set_zoom_tool();
    void clean_zoom_preview();
    void close_video_widget(VideoWidget*);
    void new_bookmark(VideoProject*, VideoState, cv::Mat, cv::Mat, QString, QString);
    void set_detections_on_frame(int);
    void start_analysis(VideoProject*, AnalysisSettings*);
    void add_tag(VideoProject*, Tag*);
    void add_interval(VideoProject*, Interval*);
    void set_interval_area(std::vector<std::pair<int, int>> area_list, int new_area_start);
    void tag_new_frame(int, TagFrame*);
    void tag_remove_frame(int);
    void set_status_bar(QString);
    void export_original_frame(VideoProject*, const int, cv::Mat);
    void delete_sc_activated();
    void zoom_preview(cv::Mat preview_frame);
    void update_manipulator_wgt(int, double, double);
public slots:
    void quick_analysis(AnalysisSettings* settings);
    void set_current_time(int time);
    void set_total_time(int time);
    void set_scale_factor(double);
    void set_zoom_state(QPoint, double, int);
    void play_btn_toggled(bool status);
    void update_tag();
    void update_tag_color(int b, double c, double g);
    void tag_frame();
    void remove_tag_frame(void);
    void new_tag_clicked();
    void new_tag(QString name);
    void new_interval(QString name);
    void set_basic_analysis(BasicAnalysis*);
    void clear_tag(void);
    void next_poi_btn_clicked(void);
    void prev_poi_btn_clicked(void);
    void analysis_play_btn_toggled(bool value);
    void on_new_frame();
    void on_playback_slider_pressed(void);
    void on_playback_slider_released(void);
    void on_playback_slider_value_changed(void);
    void on_playback_slider_moved(void);

    void load_marked_video_state(VideoProject *vid_proj, VideoState state);
    void set_item_type(int);
    void clear_current_video();
    void remove_item(VideoProject* vid_proj);

    void on_export_frame(void);
    void on_bookmark_clicked(void);
    void quick_bookmark(void);
    void set_interval_start_clicked();
    void set_interval_end_clicked();
    void create_interval_clicked();
    void delete_interval(void);
    void frame_line_edit_finished();
    void zoom_label_finished();
    void enable_poi_btns(bool, bool);
    void on_video_info(int video_width, int video_height, int frame_rate, int last_frame);
    void on_playback_stopped(void);

    void set_overlay_removed();
    void set_show_overlay(bool show);
    void set_tool(SHAPES tool);
    void set_tool_text(QString, float);
    void set_color(QColor color);
    void mouse_double_clicked(QPoint pos);
    void mouse_pressed(QPoint pos, bool);
    void mouse_released(QPoint pos, bool right_click);
    void mouse_moved(QPoint pos, bool shift, bool ctrl);
    void mouse_scroll(QPoint pos);
    void set_current_drawing(Shapes* shape);
    void process_frame();
    void update_overlay_settings(std::function<void ()> lambda);
    void pan(int x, int y);
    void center(QPoint, double);
    void set_zoom_area(QPoint p1, QPoint p2);
    void set_draw_area_size(QSize s);
    void set_interpolation_method(int method);
    void on_step_zoom(double step);
    void set_zoom_factor(double scale_factor);
    void set_state(VideoState state);
    void on_fit_screen(void);
    void on_original_size(void);
    void update_brightness_contrast(int c_val, double v_val, double g_val, bool update);
    void rotate_cw(void);
    void rotate_ccw(void);
    void update_processing_settings(std::function<void(void)> lambda);
    void update_playback_speed(int speed);
    void set_brightness_contrast(int bri, double cont, double gamma);
    void update_zoom_preview_size(QSize s);
private:
    const QSize BTN_SIZE = QSize(30, 30);
    const int PERCENT_INT_CONVERT = 100;
    const int ZOOM_LABEL_MIN = 1;
    const int ZOOM_LABEL_MAX = 10000;
    const int FIVE_SEC = 5000;

    DrawScrollArea* scroll_area;
    QSlider* speed_slider;
    QLabel* current_time;
    QLabel* total_time;
    QLabel* max_frames;
    QLineEdit* frame_line_edit;
    QLineEdit* zoom_label;
    QCheckBox* interpolate_check; // Checked = bicubic, unchecked = nearest
    QLabel* fps_label;
    QLabel* size_label;
    QLabel* rotation_label;

    //Buttons
    QPushButton* play_btn;
    QPushButton* stop_btn;
    QPushButton* next_frame_btn;
    QPushButton* prev_frame_btn;
    QPushButton* next_poi_btn;
    DoubleClickButton* prev_poi_btn;
    QPushButton* analysis_play_btn;
    QPushButton* bookmark_btn;
    QPushButton* tag_btn;
    QPushButton* new_label_btn;
    QPushButton* fit_btn;
    QPushButton* original_size_btn;
    QPushButton* set_start_interval_btn;
    QPushButton* set_end_interval_btn;
    QPushButton* export_frame_btn;
    QPushButton* create_interval_btn;

    //Shortcuts
    QShortcut* bookmark_quick_sc;
    QShortcut* interpol_sc;
    QShortcut* speed_slider_up;
    QShortcut* speed_slider_down;
    QShortcut* frame_edit_sc;
    QShortcut* zoom_edit_sc;
    QShortcut* video_start_sc;
    QShortcut* video_end_sc;
    QShortcut* page_step_front_sc;
    QShortcut* page_step_back_sc;
    QShortcut* delete_sc;

    //Layouts
    QHBoxLayout* control_row;     // Container for all button areas
    QHBoxLayout* video_btns;      // Play, pause etc
    QHBoxLayout* analysis_btns;   // Buttons for starting analysis and jumping between pois
    QHBoxLayout* other_btns;      // Bookmark, tag
    QHBoxLayout* zoom_btns;       // Zoom buttons
    QHBoxLayout* interval_btns;   // Interval buttons
    QGridLayout* speed_slider_layout;

    std::vector<QPushButton*> btns;

    QString convert_time(int time);
    VideoProject* m_vid_proj = nullptr;
    Tag* m_tag = nullptr;
    Interval* m_current_interval = nullptr;

    bool m_floating = false;
    bool state_video = false;

    bool tag_clicked = false;

    bool slider_is_blocked = false;
    bool video_btns_enabled = false;
    bool analysis_only = false;

    QString bmark_description = "";

    void set_video_btns(bool b);

    void init_control_buttons();
    void init_layouts();
    void init_video_controller();
    void init_frame_processor();

    void set_btn_icons();
    void set_btn_tool_tip();
    void set_btn_size();
    void set_btn_tab_order();
    void set_btn_shortcuts();

    void init_speed_slider();
    void add_btns_to_layouts();
    void connect_btns();
    void init_playback_slider();

    void closeEvent(QCloseEvent *event) override;
private slots:
    void stop_btn_clicked(void);
    void next_frame_clicked(void);
    void prev_frame_clicked(void);
    void speed_up_activate(void);
    void speed_down_activate(void);
    void set_video_start(void);
    void set_video_end(void);
    void page_step_front(void);
    void page_step_back(void);
    void frame_label_focus(void);
    void zoom_label_focus(void);

    void on_interpolate_toggled(bool checked);
};

#endif // VIDEOWIDGET_H
