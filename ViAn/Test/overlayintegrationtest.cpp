#include "overlayintegrationtest.h"

/**
 * @brief OverlayTester::OverlayTester
 */
OverlayIntegrationTest::OverlayIntegrationTest() {
    QMutex mutex;
    QWaitCondition wait;
    QLabel label;
    mvideo = new video_player(&mutex, &wait, &label, NULL);
}

/**
 * @brief OverlayTester::exec
 */
void OverlayIntegrationTest::exec() {

    // Setup video player
    main->mvideo_player->capture.open("seq_01.mp4");
    main->mvideo_player->frame_rate = main->mvideo_player->capture.get(CV_CAP_PROP_FPS);
    main->mvideo_player->num_frames = main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_COUNT);
    main->mvideo_player->zoom_area->set_size(main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_WIDTH), main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    main->set_pause_video();
    main->mvideo_player->frame_width = main->mvideo_player->capture.get(CV_CAP_PROP_FRAME_WIDTH);
    main->mvideo_player->frame_height = main->mvideo_player->capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    main->mvideo_player->video_frame->setFixedHeight(main->mvideo_player->capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    main->set_playback_frame(11);
    main->on_action_show_hide_overlay_triggered();

    // Draw a square
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(410, 10), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(450, 50), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(500, 100), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    // Draw an arrow
    main->on_action_arrow_triggered();
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(150, 50), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(300, 50), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    // Draw with the pen, in white
    main->on_action_pen_triggered();
    main->mvideo_player->set_overlay_colour(QColor(255, 255, 255));
    draw_pen();
    main->on_action_circle_triggered();
    // Undo a blue circle
    main->mvideo_player->set_overlay_colour(QColor(0, 255, 255));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(150, 50), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(300, 150), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->on_action_undo_triggered();

    // Zoom
    main->on_action_zoom_in_triggered();
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(10, 0), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(1000, 500), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));

    //Rotate
    main->on_action_rotate_right_triggered();
    main->on_action_rotate_right_triggered();

    // Export a bookmark
    int frame_number = main->mvideo_player->get_current_frame_num();
    QImage frame = main->mvideo_player->get_current_frame_unscaled();
    Bookmark* bookmark = new Bookmark(frame_number, frame, "integration_test1", "Bookmark Text");
    bookmark->export_frame();

    std::cout << "Overlay integration test finished.\n";
}

/**
 * @brief OverlayTester::draw_pen
 * Simulates mouse events for dragging the mouse around.
 */
void OverlayIntegrationTest::draw_pen() {
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(191, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(184, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(177, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(170, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(159, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(150, 38), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 45), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(144, 54), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 63), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 74), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 83), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 92), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 101), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 110), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(153, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(162, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 105), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 96), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(173, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(182, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(191, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(200, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(210, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(220, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(229, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(238, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(248, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(257, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(262, 94), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 107), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 130), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 141), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 154), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(260, 165), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(248, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(237, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(226, 168), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 155), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 145), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 130), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(231, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(244, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(255, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(258, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(269, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(284, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(298, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(307, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
}
