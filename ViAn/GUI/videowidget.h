#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QTime>
#include <QScrollArea>
#include <QLabel>
#include <QSize>
#include <QBoxLayout>
#include <QMutex>
#include <QWaitCondition>
#include <QPushButton>
#include <QSlider>
#include <QShortcut>
#include <vector>
#include "framewidget.h"
#include "analysisslider.h"
#include "Video/video_player.h"
#include "Project/videoproject.h"
#include "drawscrollarea.h"
#include "Project/Analysis/tag.h"
class VideoWidget : public QWidget
{
    Q_OBJECT
private:
    QScrollBar* v_bar;
    QScrollBar* h_bar;
    QSize current_frame_size;
    QTime timer;
    double h_step_size, v_step_size;
    int current_frame = 0;
    int prev_frame_idx;
    int POI_end;

    std::pair<int, int> m_interval = std::make_pair(0, 1);
public:
    explicit VideoWidget(QWidget *parent = nullptr);

    // Lock and wait condition to sleep player when video is paused
    QMutex mutex;
    QWaitCondition paused_wait;
    video_player* m_video_player;
    FrameWidget* frame_wgt;
    AnalysisSlider* playback_slider;

    VideoProject* get_current_video_project();
    std::pair<int, int> get_frame_interval();

signals:
    void first_frame(cv::Mat frame);
    void zoom_out(double zoom_factor);
    void set_zoom(double zoom_factor);
    void set_play_video(void);
    void set_pause_video(void);
    void set_stop_video(void);
    void next_video_frame(void);
    void prev_video_frame(void);
    void ret_first_frame(void);
    void set_playback_frame(int, bool);
    void new_bookmark(VideoProject*, int, cv::Mat);
    void set_detections_on_frame(int);
    void start_analysis(VideoProject*);
    void add_tag(VideoProject*, Tag*);
    void tag_updated(Tag*);
    void set_interval(int);
    void set_status_bar(QString);
public slots:
    void set_current_time(int time);
    void set_total_time(int time);
    void play_clicked(void);
    void stop_clicked(void);
    void next_frame_clicked(void);
    void prev_frame_clicked(void);
    void analysis_btn_clicked(void);
    void tag_frame(void);
    void new_tag_clicked();
    void new_tag(QString name);
    void set_tag(Tag *);
    void clear_tag(void);
    void interval_clicked(void);
    void zoom_out_clicked(void);
    void next_poi_btn_clicked(void);
    void prev_poi_btn_clicked(void);
    void analysis_play_btn_toggled(bool value);
    void set_slider_max(int value);
    void on_new_frame(int frame_num);
    void on_playback_slider_pressed(void);
    void on_playback_slider_released(void);
    void on_playback_slider_value_changed(void);
    void on_playback_slider_moved(void);
    void fit_clicked(void);
    void load_marked_video(VideoProject* vid_proj, int frame);
    void update_bar_pos(int change_x, int change_y);
    void set_current_frame_size(QSize size);
    void on_bookmark_clicked(void);
    void set_interval_start_clicked();
    void set_interval_end_clicked();
    void frame_line_edit_finished();
    void enable_poi_btns(bool, bool);
    void enable_tag_btn(bool);

private:
    const QSize BTN_SIZE = QSize(30, 30);

    QVBoxLayout* vertical_layout;
    DrawScrollArea* scroll_area;
    QSlider* speed_slider;
    QLabel* current_time;
    QLabel* total_time;
    QLineEdit* frame_line_edit;


    //Buttons
    QPushButton* play_btn;
    QPushButton* stop_btn;
    QPushButton* next_frame_btn;
    QPushButton* prev_frame_btn;
    QPushButton* next_poi_btn;
    QPushButton* prev_poi_btn;
    QPushButton* analysis_btn;
    QPushButton* analysis_play_btn;
    QPushButton* bookmark_btn;    
    QPushButton* tag_btn;
    QPushButton* new_tag_btn;
    QPushButton* interval_btn;
    QPushButton* zoom_in_btn;
    QPushButton* zoom_out_btn;
    QPushButton* fit_btn;
    QPushButton* move_btn;
    QPushButton* set_start_interval_btn;
    QPushButton* set_end_interval_btn;

    //Layouts
    QHBoxLayout* control_row;     // Container for all button areas
    QHBoxLayout* video_btns;      // Play, pause etc
    QHBoxLayout* analysis_btns;   // Buttons for starting analysis and jumping between pois
    QHBoxLayout* other_btns;      // Bookmark, tag
    QHBoxLayout* zoom_btns;       // Zoom buttons
    QGridLayout* speed_slider_layout;
    
    std::vector<QPushButton*> btns;

    QString convert_time(int time);
    VideoProject* m_vid_proj = nullptr;
    Tag* m_tag = nullptr;

    bool tag_clicked = false;

    bool slider_is_blocked = false;
    bool video_btns_enabled = false;
    bool analysis_only = false;

    void enable_video_btns();

    void init_control_buttons();

    void init_layouts();
    void set_btn_icons();
    void set_btn_tool_tip();
    void set_btn_size();
    void set_btn_tab_order();
    void set_btn_shortcuts();
    void init_speed_slider();
    void add_btns_to_layouts();
    void connect_btns();

    void init_playback_slider();

};

#endif // VIDEOWIDGET_H
