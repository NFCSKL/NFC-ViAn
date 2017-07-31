#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QTimer>
#include <mutex>

#include <opencv2/core/core.hpp>

#include "videoplayer.h"
#include "Video/frameprocessor.h"
#include <atomic>
class VideoController : public QThread {
    Q_OBJECT
    QTimer* frame_timer;
    std::atomic<int>* m_frame;
    std::atomic_int* m_width;
    std::atomic_int* m_height;

    std::atomic<bool>* m_is_playing;
    std::atomic_bool* m_new_video;
    std::atomic_bool* m_new_frame;

    video_sync* m_v_sync;
public:
    VideoController(std::atomic<int>* frame_index, std::atomic<bool>* is_playing,
                    std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                    std::atomic_bool* new_video, video_sync* v_sync);

signals:
    // TO VIDEO PLAYER
    void load_video(std::string video_path);
    void update_speed(int steps);

    // FROM VIDEO PLAYER
    void display_index();
    void video_info(int frame_width, int frame_height, int frame_rate, int last_frame);
    void playback_stopped(void);
protected:
    void run();
};

#endif // VIDEOCONTROLLER_H
