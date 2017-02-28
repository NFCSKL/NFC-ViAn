#ifndef TEST_VIDEO_PLAYER_H
#define TEST_VIDEO_PLAYER_H

#include <QObject>
#include <QTest>

#include "Video/video_player.h"

class test_video_player : public QObject
{
    Q_OBJECT
public:
    explicit test_video_player(QObject *parent = 0);
private slots:
    void test_load_video();
    void test_play();
    void test_pause();
    void test_is_paused();
    void test_get_num_frames();
    void test_set_frame_width();
    void test_set_frame_height();

private:
    video_player* mvideo;
signals:

public slots:
};
#endif // TEST_VIDEO_PLAYER_H
