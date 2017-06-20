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
#include <QShortcut>
#include <vector>
#include "framewidget.h"
#include "analysisslider.h"
#include "Video/video_player.h"

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

    // Lock and wait condition to sleep player when video is paused
    QMutex mutex;
    QWaitCondition paused_wait;

    video_player* m_video_player;

signals:
    set_pause_video();
    set_stop_video();
    next_video_frame();
    prev_video_frame();

    set_playback_frame(int, bool);

public slots:
    void set_current_time(int time);
    void set_total_time(int time);
    void play_clicked(void);
    void stop_clicked(void);
    void next_frame_clicked(void);
    void prev_frame_clicked(void);
    void zoom_in_clicked();
    void zoom_out_clicked();
    void set_slider_max(int value);
    void on_new_frame(int frame_num);
    void on_playback_slider_pressed(void);
    void on_playback_slider_released(void);
    void on_playback_slider_value_changed(void);
    void on_playback_slider_moved(void);
    //void next_poi_clicked(void);
    //void prev_poi_clicked(void);

private:
    const QSize BTN_SIZE = QSize(30, 30);

    QVBoxLayout* vertical_layout;
    QScrollArea* scroll_area;
    QSlider* speed_slider;
    AnalysisSlider* playback_slider;
    QLabel* current_time;
    QLabel* total_time;
    FrameWidget* frame_wgt;

    //Buttons
    QPushButton* play_btn;
    QPushButton* stop_btn;
    QPushButton* next_frame_btn;
    QPushButton* prev_frame_btn;
    QPushButton* next_poi_btn;
    QPushButton* prev_poi_btn;
    QPushButton* analysis_btn;
    QPushButton* bookmark_btn;    
    QPushButton* tag_btn;
    QPushButton* zoom_in_btn;
    QPushButton* zoom_out_btn;
    QPushButton* fit_btn;
    QPushButton* move_btn;

    //Layouts
    QHBoxLayout* control_row;     // Container for all button areas
    QHBoxLayout* video_btns;      // Play, pause etc
    QHBoxLayout* analysis_btns;   // Buttons for starting analysis and jumping between pois
    QHBoxLayout* other_btns;      // Bookmark, tag
    QHBoxLayout* zoom_btns;       // Zoom buttons
    QGridLayout* speed_slider_layout;

    //Shortcuts
    QShortcut* play_sc;
    QShortcut* stop_sc;
    QShortcut* next_frame_sc;
    QShortcut* prev_frame_sc;
    QShortcut* next_poi_sc;
    QShortcut* prev_poi_sc;
    
    std::vector<QPushButton*> btns;

    QString convert_time(int time);

    bool slider_is_blocked = false;

    void init_control_buttons();

    void init_layouts();
    void set_btn_icons();
    void set_btn_tool_tip();
    void set_btn_size();
    void set_btn_tab_order();
    void set_btn_shortcuts();

    void init_speed_slider();

    void add_btns_to_layouts();

    void init_playback_slider();

};

#endif // VIDEOWIDGET_H
