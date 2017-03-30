#include "test_video_player.h"
#include <thread>
#include <QMutex>
#include <QWaitCondition>

/**
 * @brief test_video_player::test_video_player
 * constructor
 * @param parent
 */
test_video_player::test_video_player(QObject *parent) : QObject(parent) {
    QMutex mutex;
    QWaitCondition wait;
    mvideo = new video_player(&mutex, &wait);
}

/**
 * @brief test_video_player::test_load_video
 */
void test_video_player::test_load_video() {
    string working_filename = "seq_01.mp4";
    string non_existing_filename = "ex.biz";
    string existing_non_video = "Makefile";

    QVERIFY(mvideo->capture.open(working_filename));
    mvideo->frame_rate = mvideo->capture.get(CV_CAP_PROP_FPS);
    mvideo->num_frames = mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT);
    mvideo->video_paused = false;

    QVERIFY(mvideo->capture.isOpened());

}

/**
 * @brief test_video_player::test_stop_video
 * Tests ONLY the case when the video is paused because if the video
 * is running, the stop related commands are executed on another thread.
 */
void test_video_player::test_stop_video() {
    mvideo->video_paused = true;
    mvideo->video_stopped = false;
    mvideo->current_frame = 50;
    mvideo->stop_video();
    QVERIFY(mvideo->video_stopped == true);
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
 * @brief test_video_player::test_next_frame
 * Currently this method cannot be tested because of thread issues.
 */
void test_video_player::test_next_frame() {
    /*
    mvideo->load_video("seq_01.mp4");
    mvideo->set_playback_frame(100);
    mvideo->next_frame();
    QVERIFY(mvideo->current_frame == 101);
    */
}

/**
 * @brief test_video_player::test_previous_frame
 * Currently this method cannot be tested because of thread issues.
 */
void test_video_player::test_previous_frame() {
    /*
    mvideo->load_video("seq_01.mp4");
    mvideo->set_playback_frame(100);
    mvideo->previous_frame();
    QVERIFY(mvideo->current_frame == 99);
    */
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

/**
 * @brief test_toggle_overlay
 */
void test_video_player::test_toggle_overlay() {
    mvideo->video_overlay->set_showing_overlay(false);
    mvideo->toggle_overlay();
    QVERIFY(mvideo->is_showing_overlay());
    mvideo->toggle_overlay();
    QVERIFY(!mvideo->is_showing_overlay());
}

/**
 * @brief test_video_player::test_set_overlay_tool
 */
void test_video_player::test_set_overlay_tool() {
    mvideo->set_overlay_tool(RECTANGLE);
    QVERIFY(mvideo->video_overlay->get_shape() == RECTANGLE);
}

/**
 * @brief test_video_player::test_set_overlay_colour
 */
void test_video_player::test_set_overlay_colour() {
    mvideo->set_overlay_colour(Qt::black);
    QVERIFY(mvideo->video_overlay->get_colour() == Qt::black);
}

/**
 * @brief test_video_player::test_video_open
 */
void test_video_player::test_video_open() {
    mvideo->capture.release();
    QVERIFY(!mvideo->video_open());
    mvideo->capture.open("seq_01.mp4");
    QVERIFY(mvideo->video_open());
}

/**
 * @brief test_video_player::test_scaling_event
 * Tests that the aspect ratio is kept when the QLabel signals a scaling event.
 */
void test_video_player::test_scaling_event() {
    mvideo->capture.release();
    mvideo->capture.open("seq_01.mp4");
    mvideo->scaling_event(640,1080);

    QVERIFY(mvideo->frame_height < 1080);

    mvideo->scaling_event(1920,480);
    QVERIFY(mvideo->frame_width < 1920);
}

/**
 * @brief test_video_player::test_scale_frame
 */
void test_video_player::test_scale_frame() {
    mvideo->capture.release();
    mvideo->capture.open("seq_01.mp4");
    int video_width = mvideo->capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_height = mvideo->capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    mvideo->capture.read(mvideo->frame);

    mvideo->scaling_event(video_width*2,
                          video_height*2);
    cv::Mat temp = mvideo->scale_frame(mvideo->frame);
    QVERIFY(temp.cols == video_width*2 && temp.rows == video_height*2);

    mvideo->scaling_event(0,0);
    temp = mvideo->scale_frame(mvideo->frame);
    QVERIFY(temp.cols == video_width && temp.rows == video_height);
}

/**
 * @brief test_video_player::test_set_play_video
 */
void test_video_player::test_set_play_video() {
    mvideo->video_paused = true;
    mvideo-> video_stopped = true;
    mvideo->on_play_video();
    QVERIFY(!mvideo->video_paused);
    QVERIFY(!mvideo->video_stopped);
}

/**
 * @brief test_video_player::test_set_paused_video
 */
void test_video_player::test_set_pause_video() {
    mvideo->video_paused = false;
    mvideo->on_pause_video();
    QVERIFY(mvideo->video_paused);
}

/**
 * @brief test_video_player::test_set_stop_video
 */
void test_video_player::test_set_stop_video() {
    mvideo->video_paused = true;
    mvideo->video_stopped = false;
    mvideo->on_stop_video();
    QVERIFY(!mvideo->video_paused);
    QVERIFY(mvideo->video_stopped);
}
