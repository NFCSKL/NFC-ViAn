#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QSize>
#include <QBoxLayout>
#include <QMutex>
#include <QWaitCondition>
#include <QPushButton>
#include <QSlider>
#include "framewidget.h"
#include "Video/video_player.h"
#include "Project/videoproject.h"

class VideoWidget : public QWidget
{
    Q_OBJECT
    QScrollBar* v_bar;
    QScrollBar* h_bar;
    QSize current_frame_size;
    double h_step_size, v_step_size;
public:
    explicit VideoWidget(QWidget *parent = nullptr);

    // Lock and wait condition to sleep player when video is paused
    QMutex mutex;
    QWaitCondition paused_wait;
    video_player* m_video_player;

signals:
    set_play_video();
    set_pause_video();
    set_stop_video();
    next_video_frame();
    prev_video_frame();
    ret_first_frame();
    set_playback_frame(int, bool);
    void first_frame(cv::Mat frame);
    void zoom_out(double zoom_factor);
    void set_zoom(double zoom_factor);
public slots:
    void play_clicked(void);
    void stop_clicked(void);
    void next_frame_clicked(void);
    void prev_frame_clicked(void);
    void zoom_out_clicked();
    void set_slider_max(int value);
    void on_new_frame(int frame_num);
    void on_playback_slider_pressed(void);
    void on_playback_slider_released(void);
    void on_playback_slider_value_changed(void);
    void on_playback_slider_moved(void);
    void fit_clicked(void);
    //void next_poi_clicked(void);
    //void prev_poi_clicked(void);
    void set_current_time(int time);
    void set_total_time(int time);
    void load_marked_video(VideoProject* vid_proj);
    void update_bar_pos(int change_x, int change_y);
    void set_current_frame_size(QSize size);

private:
    const QSize BTN_SIZE = QSize(30, 30);

    QVBoxLayout* vertical_layout;
    QScrollArea* scroll_area;
    QSlider* speed_slider;
    QSlider* playback_slider;
    QLabel* current_time;
    QLabel* total_time;
    FrameWidget* frame_wgt;
    QPushButton* play_btn;
    QPushButton* zoom_in_btn;
    QPushButton* fit_btn;
    QString convert_time(int time);
    VideoProject* m_vid_proj;

    bool slider_is_blocked = false;

    void init_control_buttons();
    void init_playback_slider();

};

#endif // VIDEOWIDGET_H
