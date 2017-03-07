#ifndef TEST_VIDEO_PLAYER_H
#define TEST_VIDEO_PLAYER_H

#include <QObject>
#include <QTest>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

#include "Video/video_player.h"

class test_video_player : public QObject
{
    Q_OBJECT
public:
    explicit test_video_player(QObject *parent = 0);
private slots:
    void test_load_video();
    void test_play_pause();
    void test_is_paused();
    void test_get_num_frames();
    void test_set_frame_width();
    void test_set_frame_height();
    void test_set_playback_frame();
    void test_next_frame();
    void test_previous_frame();
    void test_inc_playback_speed();
    void test_dec_playback_speed();
    void test_stop_video();


private:
    video_player* mvideo;
signals:

public slots:
};
#endif // TEST_VIDEO_PLAYER_H
