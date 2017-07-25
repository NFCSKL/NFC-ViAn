#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QTimer>

#include <opencv2/core/core.hpp>

#include "Video/frameprocessor.h"

class VideoController : public QThread {
    Q_OBJECT
    QTimer* frame_timer;
    std::atomic<int>* m_frame;
    std::atomic<bool>* m_is_playing;
public:
    VideoController(std::atomic<int>* frame, std::atomic<bool>* is_playing);

signals:
    // TO VIDEO PLAYER
    void load_video(std::string video_path);

    void update_speed(int steps);

    // FROM VIDEO PLAYER
    void display_index();
    void video_info(int frame_width, int frame_height, int frame_rate, int last_frame);
    void playback_stopped(void);

    // TO PROCESSOR
    void set_zoom_rect(QPoint p1, QPoint p2);
    void set_draw_area_size(QSize size);
    void zoom_out();
    void fit_screen();
    void move_zoom_rect(int x, int y);
    void rotate_right();
    void rotate_left();
    void set_bright_cont(int b_val, double c_val);

    // FROM PROCESSOR
    void display_image(cv::Mat frame, int frame_index);

protected:
    void run();
};

#endif // VIDEOCONTROLLER_H
