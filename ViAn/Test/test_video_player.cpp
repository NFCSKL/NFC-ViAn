//#include "test_video_player.h"
//#include <thread>
//#include <QMutex>
//#include <QWaitCondition>

///**
// * @brief test_video_player::test_video_player
// * constructor
// * @param parent
// */
//test_video_player::test_video_player(QObject *parent) : QObject(parent) {
//    QMutex mutex;
//    QWaitCondition wait;
//    mvideo = new video_player(&mutex, &wait, NULL);
//    mvideo->video_overlay = new Overlay();
//}

///**
// * @brief test_video_player::test_load_video
// */
//void test_video_player::test_load_video() {
//    string working_filename = "seq_01.mp4";
//    string non_existing_filename = "ex.biz";
//    string existing_non_video = "Makefile";

//    QVERIFY(mvideo->capture.open(working_filename));
//    mvideo->frame_rate = mvideo->capture.get(CV_CAP_PROP_FPS);
//    mvideo->num_frames = mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT);
//    mvideo->video_paused = false;

//    QVERIFY(mvideo->capture.isOpened());

//}

///**
// * @brief test_video_player::test_is_paused
// */
//void test_video_player::test_is_paused() {
//    mvideo->video_paused = false;
//    QVERIFY(!mvideo->is_paused());
//    mvideo->video_paused = true;
//    QVERIFY(mvideo->is_paused());
//}

///**
// * @brief test_video_player::test_get_num_frames
// */
//void test_video_player::test_get_num_frames() {
//    QVERIFY(mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT) == mvideo->get_num_frames());
//}

///**
// * @brief test_video_player::test_set_frame_width
// */
//void test_video_player::test_set_frame_width() {
//    mvideo->set_frame_width(50);
//    QVERIFY(mvideo->frame_width == 50);
//}

///**
// * @brief test_video_player::test_set_frame_height
// */
//void test_video_player::test_set_frame_height() {
//    mvideo->set_frame_height(50);
//    QVERIFY(mvideo->frame_height == 50);
//}

/////**
//// * @brief test_video_player::test_next_frame
//// */
////void test_video_player::test_next_frame() {
////    mvideo->video_paused = true;
////    mvideo->set_current_frame_num(100);
////    mvideo->next_frame();
////    QVERIFY(mvideo->get_current_frame_num() == 101);
////}

/////**
//// * @brief test_video_player::test_previous_frame
//// */
////void test_video_player::test_previous_frame() {
////    mvideo->video_paused = true;
////    mvideo->set_current_frame_num(100);
////    mvideo->previous_frame();
////    QVERIFY(mvideo->get_current_frame_num() == 99);
////}

///**
// * @brief test_video_player::test_inc_playback_speed
//void test_video_player::test_inc_playback_speed(){
//    mvideo->set_speed_multiplier(1);
//    QVERIFY(mvideo->get_speed_multiplier() == 1);

//    mvideo->inc_playback_speed();
//    QVERIFY(mvideo->get_speed_multiplier() == 0.5);

//    mvideo->set_speed_multiplier(1.0/16);
//    mvideo->inc_playback_speed();
//    QVERIFY(mvideo->get_speed_multiplier() == 1.0/16);
//}
//*/

///**
// * @brief test_video_player::test_dec_playback_speed
//void test_video_player::test_dec_playback_speed(){
//    mvideo->set_speed_multiplier(1);
//    QVERIFY(mvideo->get_speed_multiplier() == 1);

//    mvideo->dec_playback_speed();
//    QVERIFY(mvideo->get_speed_multiplier() == 2);

//    mvideo->set_speed_multiplier(16);
//    mvideo->dec_playback_speed();
//    QVERIFY(mvideo->get_speed_multiplier() == 16);
//}
//*/

///**
// * @brief test_toggle_overlay
// */
//void test_video_player::test_toggle_overlay() {
//    QMutex mutex;
//    QWaitCondition wait;
//    QLabel label;
//    video_player *v_player = new video_player(&mutex, &wait, &label);
//    v_player->video_overlay = new Overlay();
//    v_player->video_overlay->set_showing_overlay(false);
//    v_player->toggle_overlay();
//    QVERIFY(v_player->is_showing_overlay());
//    v_player->toggle_overlay();
//    QVERIFY(!v_player->is_showing_overlay());
//    bool original_value = v_player->analysis_overlay->is_showing_overlay();
//    v_player->toggle_analysis_overlay();
//    QVERIFY(v_player->is_showing_analysis_overlay() != original_value);
//    v_player->toggle_analysis_overlay();
//    QVERIFY(v_player->is_showing_analysis_overlay() == original_value);
//}

///**
// * @brief test_video_player::test_set_overlay_tool
// */
//void test_video_player::test_set_overlay_tool() {
//    mvideo->set_overlay_tool(RECTANGLE);
//    QVERIFY(mvideo->video_overlay->get_tool() == RECTANGLE);
//}

///**
// * @brief test_video_player::test_set_overlay_colour
// */
//void test_video_player::test_set_overlay_colour() {
//    mvideo->set_overlay_colour(Qt::black);
//    QVERIFY(mvideo->video_overlay->get_colour() == Qt::black);
//}

///**
// * @brief test_video_player::test_reset_brightness_contrast
// */
////void test_video_player::test_reset_brightness_contrast() {
////    mvideo->reset_brightness_contrast();
////    QVERIFY(mvideo->CONTRAST_DEFAULT);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_DEFAULT);
////}

///**
// * @brief test_video_player::test_set_contrast
// */
////void test_video_player::test_set_contrast() {
////    mvideo->set_contrast(mvideo->CONTRAST_MIN - 10);
////    QVERIFY(mvideo->CONTRAST_MIN == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MIN - 0.01);
////    QVERIFY(mvideo->CONTRAST_MIN == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MIN);
////    QVERIFY(mvideo->CONTRAST_MIN == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MIN + 0.1);
////    QVERIFY(mvideo->CONTRAST_MIN + 0.1 == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_DEFAULT);
////    QVERIFY(mvideo->CONTRAST_DEFAULT == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MAX - 0.1);
////    QVERIFY(mvideo->CONTRAST_MAX - 0.1 == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MAX);
////    QVERIFY(mvideo->CONTRAST_MAX == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MAX + 0.01);
////    QVERIFY(mvideo->CONTRAST_MAX == mvideo->get_contrast());
////    mvideo->set_contrast(mvideo->CONTRAST_MAX + 10);
////    QVERIFY(mvideo->CONTRAST_MAX == mvideo->get_contrast());

////    // Values should be doubles 0.5 to 5.0
////    mvideo->set_contrast(-10);
////    QVERIFY(0.5 == mvideo->get_contrast());
////    mvideo->set_contrast(0);
////    QVERIFY(0.5 == mvideo->get_contrast());
////    mvideo->set_contrast(0.49);
////    QVERIFY(0.5 == mvideo->get_contrast());
////    mvideo->set_contrast(0.5);
////    QVERIFY(0.5 == mvideo->get_contrast());
////    mvideo->set_contrast(0.51);
////    QVERIFY(0.51 == mvideo->get_contrast());
////    mvideo->set_contrast(1);
////    QVERIFY(1 == mvideo->get_contrast());
////    mvideo->set_contrast(2);
////    QVERIFY(2 == mvideo->get_contrast());
////    mvideo->set_contrast(4.99);
////    QVERIFY(4.99 == mvideo->get_contrast());
////    mvideo->set_contrast(5);
////    QVERIFY(5 == mvideo->get_contrast());
////    mvideo->set_contrast(5.01);
////    QVERIFY(5 == mvideo->get_contrast());
////    mvideo->set_contrast(500);
////    QVERIFY(5 == mvideo->get_contrast());
////}

///**
// * @brief test_video_player::test_set_brightness
// */
////void test_video_player::test_set_brightness() {
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN - 10);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN - 1);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN + 1);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN + 1);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_DEFAULT);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_DEFAULT);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX - 1);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX - 1);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX + 1);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);
////    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX + 10);
////    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);

////    // Values should be integers -100 to 100
////    mvideo->set_brightness(-101);
////    QVERIFY(mvideo->get_brightness() == -100);
////    mvideo->set_brightness(-100);
////    QVERIFY(mvideo->get_brightness() == -100);
////    mvideo->set_brightness(-99);
////    QVERIFY(mvideo->get_brightness() == -99);
////    mvideo->set_brightness(0);
////    QVERIFY(mvideo->get_brightness() == 0);
////    mvideo->set_brightness(1);
////    QVERIFY(mvideo->get_brightness() == 1);
////    mvideo->set_brightness(2);
////    QVERIFY(mvideo->get_brightness() == 2);
////    mvideo->set_brightness(99);
////    QVERIFY(mvideo->get_brightness() == 99);
////    mvideo->set_brightness(100);
////    QVERIFY(mvideo->get_brightness() == 100);
////    mvideo->set_brightness(101);
////    QVERIFY(mvideo->get_brightness() == 100);
////    mvideo->set_brightness(500);
////    QVERIFY(mvideo->get_brightness() == 100);
////}

///**
// * @brief test_video_player::test_video_open
// */
//void test_video_player::test_video_open() {
//    mvideo->capture.release();
//    QVERIFY(!mvideo->video_open());
//    mvideo->capture.open("seq_01.mp4");
//    QVERIFY(mvideo->video_open());
//}

///**
// * @brief test_video_player::test_set_play_video
// */
//void test_video_player::test_set_play_video() {
//    mvideo->video_paused = true;
//    mvideo-> video_stopped = true;
//    mvideo->on_play_video();
//    QVERIFY(!mvideo->video_paused);
//    QVERIFY(!mvideo->video_stopped);
//}

///**
// * @brief test_video_player::test_set_paused_video
// */
//void test_video_player::test_set_pause_video() {
//    mvideo->video_paused = false;
//    mvideo->on_pause_video();
//    QVERIFY(mvideo->video_paused);
//}

///**
// * @brief test_video_player::test_set_stop_video
// */
//void test_video_player::test_set_stop_video() {
//    mvideo->video_paused = true;
//    mvideo->video_stopped = false;
//    mvideo->on_stop_video();
//    QVERIFY(!mvideo->video_paused);
//    QVERIFY(mvideo->get_current_frame_num() == 0);
//    QVERIFY(mvideo->video_stopped);
//}

///**
// * @brief test_video_player::test_analysis_area_toggle
// */
//void test_video_player::test_analysis_area_toggle() {
//    bool original_value = mvideo->analysis_area->is_including_area();
//    mvideo->analysis_area->invert_area();
//    QVERIFY(mvideo->analysis_area->is_including_area() != original_value);
//}

///**
// * @brief test_video_player::test_analysis_area_points
// */
//void test_video_player::test_analysis_area_points() {
//    std::vector<QPoint> original_points;
//    original_points.push_back(QPoint(0, 10));
//    original_points.push_back(QPoint(210, 150));
//    original_points.push_back(QPoint(255, 255));
//    original_points.push_back(QPoint(1024, 0));
//    for (std::vector<int>::size_type i = 0; i != original_points.size(); i++) {
//        mvideo->analysis_area->add_point(original_points[i]);
//    }
//    std::vector<cv::Point>* points = mvideo->analysis_area->get_polygon();
//    for (std::vector<int>::size_type i = 0; i != points->size(); i++) {
//        QVERIFY((*points)[i].x == original_points[i].x());
//    }
//}

///**
// * @brief test_video_player::test_on_set_playback_frame_pass
// * Test on_set_playback_frame with valid inputs
//void test_video_player::test_on_set_playback_frame_pass() {
//    mvideo->video_paused = false;
//    mvideo->on_set_playback_frame(100);
//    QVERIFY(mvideo->new_frame_num == 100);
//    QVERIFY(mvideo->set_new_frame);

//    mvideo->video_paused = true;
//    mvideo->on_set_playback_frame(100);
//    QVERIFY(mvideo->capture.get(CV_CAP_PROP_POS_FRAMES) == 100);
//}
//*/

///**
// * @brief test_video_player::test_on_set_playback_frame_fail
// * Test on_set_playback_frame with non valid inputs
//void test_video_player::test_on_set_playback_frame_fail() {
//    int out_of_bounds = mvideo->get_num_frames() + 100;
//    int test_frames[] = {-100, out_of_bounds};
//    for (int &frame : test_frames) {
//        mvideo->video_paused = false;
//        mvideo->on_set_playback_frame(frame);
//        QVERIFY(mvideo->new_frame_num != frame);
//        QVERIFY(!mvideo->set_new_frame);

//        mvideo->video_paused = true;
//        mvideo->on_set_playback_frame(frame);
//        QVERIFY(mvideo->capture.get(CV_CAP_PROP_POS_FRAMES) != frame);
//    }
//}
//*/


/////**
//// * @brief test_video_player::test_set_current_frame_num
//// * Testing if setting the current frame to a specific frame in a video works as expected.
//// */
////void test_video_player::test_set_current_frame_num() {
////    mvideo->capture.release();
////    mvideo->capture.open("seq_01.mp4");

////    const int LEGAL_FRAME = 100;
////    const int ILLEGAL_FRAME = 999999;

////    //Testing different branches with legal frame number
////    mvideo->frame.release();
////    mvideo->video_paused = true;
////    QVERIFY(mvideo->set_current_frame_num(LEGAL_FRAME));
////    QVERIFY(mvideo->capture.get(CV_CAP_PROP_POS_FRAMES) == LEGAL_FRAME+1);
////    QVERIFY(mvideo->frame.elemSize()*mvideo->frame.cols*mvideo->frame.rows != 0);

////    mvideo->video_paused = false;
////    mvideo->new_frame_num = -1;
////    mvideo->set_new_frame = false;
////    mvideo->set_current_frame_num(LEGAL_FRAME);
////    QVERIFY(mvideo->new_frame_num == LEGAL_FRAME && mvideo->set_new_frame);

////    //Testing illegal frame number
////    QVERIFY(!mvideo->set_current_frame_num(ILLEGAL_FRAME));
////}

///**
// * @brief test_video_player::test_convert_frame
// * Testing if frames are converted correctly.

//void test_video_player::test_convert_frame() {
//    mvideo->capture.release();
//    mvideo->frame.release();
//    mvideo->img = QPixmap(0, 0).toImage();
//    mvideo->capture.open("seq_01.mp4");

//    //Testing with empty frame
//    mvideo->frame = cv::Mat(0,0,CV_8U);
//    mvideo->convert_frame(true);
//    QVERIFY(mvideo->img.byteCount() == 0);

//    //Testing with color frame
//    mvideo->capture.read(mvideo->frame);
//    mvideo->convert_frame(false);
//    QVERIFY(mvideo->img.format() == QImage::Format_RGB888);
//    QVERIFY(mvideo->img.width() == mvideo->frame.cols && mvideo->img.height() == mvideo->frame.rows);

//    //Testing with grayscale frame
//    mvideo->capture.read(mvideo->frame);
//    cv::Mat grayMat;
//    cv::cvtColor(mvideo->frame, grayMat, cv::COLOR_BGR2GRAY);
//    grayMat.copyTo(mvideo->frame);
//    grayMat.release();
//    mvideo->convert_frame(false);
//    QVERIFY(mvideo->img.format() == QImage::Format_Indexed8);
//    QVERIFY(mvideo->img.width() == mvideo->frame.cols && mvideo->img.height() == mvideo->frame.rows);
//}
//*/

///**
// * @brief test_video_player::test_set_zoom_area

//void test_video_player::test_set_zoom_area() {
//    mvideo->zoom_area->set_zoom_area(0, 0, 10, 10);
//    QVERIFY(mvideo->zoom_area->get_x() == 0);
//    QVERIFY(mvideo->zoom_area->get_y() == 0);
//    QVERIFY(mvideo->zoom_area->get_width() == 10);
//    QVERIFY(mvideo->zoom_area->get_height() == 10);
//    QVERIFY(mvideo->zoom_area->get_zoom_area().x == 0);
//    QVERIFY(mvideo->zoom_area->get_zoom_area().y == 0);
//    QVERIFY(mvideo->zoom_area->get_zoom_area().width == 10);
//    QVERIFY(mvideo->zoom_area->get_zoom_area().height == 10);
//}
//*/

///**
// * @brief test_reset_zoom_area
//void test_video_player::test_reset_zoom_area() {
//    mvideo->zoom_area->set_zoom_area(10, 10, 100, 100);
//    mvideo->zoom_area->set_size(128, 128);
//    QVERIFY(mvideo->zoom_area->get_x() == 0);
//    QVERIFY(mvideo->zoom_area->get_y() == 0);
//    QVERIFY(mvideo->zoom_area->get_width() == 128);
//    QVERIFY(mvideo->zoom_area->get_height() == 128);
//}
//*/
