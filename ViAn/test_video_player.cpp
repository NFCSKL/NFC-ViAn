#include "test_video_player.h"

test_video_player::test_video_player(QObject *parent) : QObject(parent) {
    mvideo = new video_player();
}

void test_video_player::test_load_video() {
    string working_filename = "seq_01.mp4";
    string non_existing_filename = "ex.biz";
    string existing_non_video = "Makefile";

    QVERIFY(mvideo->load_video(working_filename));
    QVERIFY(mvideo->capture.isOpened());

}


void test_video_player::test_play() {
    mvideo->play();
    QVERIFY(!mvideo->video_paused);
}

void test_video_player::test_pause() {
    mvideo->pause();
    QVERIFY(mvideo->video_paused);
}

void test_video_player::test_is_paused() {
    mvideo->video_paused = false;
    QVERIFY(!mvideo->is_paused());
    mvideo->video_paused = true;
    QVERIFY(mvideo->is_paused());
}

void test_video_player::test_get_num_frames() {
    QVERIFY(mvideo->capture.get(CAP_PROP_FRAME_COUNT) == mvideo->get_num_frames());
}

void test_video_player::test_set_frame_width() {
    mvideo->set_frame_width(50);
    QVERIFY(mvideo->frame_width == 50);
}

void test_video_player::test_set_frame_height() {
    mvideo->set_frame_height(50);
    QVERIFY(mvideo->frame_height == 50);
}


