#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QThread>
#include <condition_variable>
#include <mutex>


class VideoPlayer;
struct video_sync;

/**
 * @brief The VideoController class
 * Video controller is used as a forwarding interface which controlls
 * the Videoplayer thread.
 */
class VideoController : public QThread {
    Q_OBJECT
    std::atomic<int>* m_frame;
    std::atomic_int* m_width;
    std::atomic_int* m_height;
    std::atomic_int* m_speed;


    std::atomic_bool* m_is_playing;
    std::atomic_bool* m_new_video;
    std::atomic_bool* m_new_frame_video;
    std::atomic_bool* m_new_frame;
    std::atomic_bool* m_video_loaded;
    std::atomic_bool* m_abort_playback;

    std::condition_variable* m_player_con;
    std::mutex* m_player_lock;

    QString* m_video_path;

    video_sync* m_v_sync;

    VideoPlayer* v_player;
public:
    VideoController(std::atomic<int>* frame_index, std::atomic_bool *is_playing,
                    std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                    std::atomic_bool* new_video, std::atomic_bool *new_frame_video, std::atomic_bool* video_loaded, video_sync* v_sync, std::condition_variable* player_con,
                    std::mutex* player_lock, QString *video_path, std::atomic_int* speed, std::atomic_bool* abort_playback);

    ~VideoController();


signals:
    // FROM VIDEO PLAYER
    void display_index();
    void capture_failed();
    void video_info(int frame_width, int frame_height, int frame_rate, int last_frame);
    void playback_stopped(void);
protected:
    void run();
};

#endif // VIDEOCONTROLLER_H
