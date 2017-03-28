#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QImageWriter>
#include <QWaitCondition>
#include "overlay.h"

#include <chrono>

using namespace std;


class video_player : public QThread {
    Q_OBJECT
public:
    video_player(QObject* parent = 0);
    ~video_player();
    bool load_video(string filename);
    bool is_paused();
    bool is_stopped();
    bool is_showing_overlay();
    void export_current_frame(QString path_to_folder);

    int get_num_frames();    
    void play_pause();
    void stop_video();
    void set_frame_width(int new_value);
    void set_frame_height(int new_value);
    bool set_playback_frame(int frame_num);
    void next_frame();
    void previous_frame();
    void set_speed_multiplier(double mult);

    double get_speed_multiplier();

    void inc_playback_speed();
    void dec_playback_speed();
    
    void toggle_overlay();
    void set_overlay_tool(SHAPES shape);
    void set_overlay_colour(QColor colour);
    void undo_overlay();
    void clear_overlay();
    void zoom_in();
    void zoom_out();
    void video_mouse_pressed(QPoint pos);
    void video_mouse_released(QPoint pos);
    void video_mouse_moved(QPoint pos);

    friend class test_video_player;

    const double MAX_SPEED_MULT = 16;
    const double MIN_SPEED_MULT = 1.0/16;
    const double DEFAULT_SPEED_MULT = 1;
    const double SPEED_STEP_MULT = 2;

signals:
    void processed_image(const QImage &image);
    void update_current_frame(const int frame);

private slots:
    void scaling_event(int width, int height);

protected:
    void run() override;
    void msleep(int ms);

private:
    void update_frame(int frame_nbr);
    cv::Mat zoom_frame(cv::Mat &frame);
    cv::Mat scale_frame(cv::Mat &src);
    void update_overlay();
    void show_frame();
    void convert_frame();
    void scale_position(QPoint &pos);

    cv::VideoCapture capture;
    cv::Mat frame;
    cv::Mat RGBframe;

    int num_frames;
    unsigned int current_frame = 0;
    unsigned int frame_width;
    unsigned int frame_height;

    double frame_rate;
    double speed_multiplier = DEFAULT_SPEED_MULT;

    bool stop = false;
    bool video_paused;
    bool choosing_zoom_area = false;

    QImage img;
    QWaitCondition condition;

    zoomrectangle* zoom_area = new zoomrectangle();

    overlay* video_overlay;
};

#endif // VIDEO_PLAYER_H
