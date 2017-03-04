#include "test_video_player.h"
#include <thread>

/**
 * @brief test_video_player::test_video_player
 * constructor
 * @param parent
 */
test_video_player::test_video_player(QObject *parent) : QObject(parent) {
    mvideo = new video_player();
}

/**
 * @brief test_video_player::test_load_video
 */
void test_video_player::test_load_video() {
    string working_filename = "seq_01.mp4";
    string non_existing_filename = "ex.biz";
    string existing_non_video = "Makefile";

    QVERIFY(mvideo->load_video(working_filename));
    QVERIFY(mvideo->capture.isOpened());

}

/**
 * @brief test_video_player::test_play_pause
 */
void test_video_player::test_play_pause() {
    mvideo->video_paused = false;
    mvideo->play_pause();
    QVERIFY(mvideo->is_paused());
    mvideo->play_pause();
    QVERIFY(!mvideo->is_paused());
}

/**
 * @brief test_video_player::test_stop_video
 * Tests ONLY the case when the video is paused because if the video
 * is running, the stop related commands are executed on another thread.
 */
void test_video_player::test_stop_video() {
    mvideo->video_paused = true;
    mvideo->stop = false;
    mvideo->current_frame = 50;
    mvideo->stop_video();
    QVERIFY(mvideo->stop == true);
    QVERIFY(mvideo->current_frame == 0);
    QVERIFY(mvideo->video_paused == false);
}

/**
 * @brief test_video_player::test_is_paused
 */
void test_video_player::test_is_paused() {
    mvideo->video_paused = false;
    QVERIFY(!mvideo->is_paused());
    mvideo->video_paused = true;
    QVERIFY(mvideo->is_paused());
}

/**
 * @brief test_video_player::test_get_num_frames
 */
void test_video_player::test_get_num_frames() {
    QVERIFY(mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT) == mvideo->get_num_frames());
}

/**
 * @brief test_video_player::test_set_frame_width
 */
void test_video_player::test_set_frame_width() {
    mvideo->set_frame_width(50);
    QVERIFY(mvideo->frame_width == 50);
}

/**
 * @brief test_video_player::test_set_frame_height
 */
void test_video_player::test_set_frame_height() {
    mvideo->set_frame_height(50);
    QVERIFY(mvideo->frame_height == 50);
}

/**
 * @brief test_video_player::test_set_playback_frame
 */
void test_video_player::test_set_playback_frame() {
    mvideo->set_playback_frame(100);
    QVERIFY(mvideo->current_frame == 100);
}

/**
 * @brief test_video_player::test_inc_playback_speed
 */
void test_video_player::test_inc_playback_speed(){
    mvideo->set_speed_multiplier(1);
    QVERIFY(mvideo->get_speed_multiplier() == 1);

    mvideo->inc_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 0.5);

    mvideo->set_speed_multiplier(1.0/16);
    mvideo->inc_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 1.0/16);
}

/**
 * @brief test_video_player::test_dec_playback_speed
 */
void test_video_player::test_dec_playback_speed(){
    mvideo->set_speed_multiplier(1);
    QVERIFY(mvideo->get_speed_multiplier() == 1);

    mvideo->dec_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 2);

    mvideo->set_speed_multiplier(16);
    mvideo->dec_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 16);
}


