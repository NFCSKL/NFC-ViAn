#include "overlaytester.h"

/**
 * @brief OverlayTester::OverlayTester
 */
OverlayTester::OverlayTester() {
    QMutex mutex;
    QWaitCondition wait;
    QLabel label;
    mvideo = new video_player(&mutex, &wait, &label, NULL);
}

/**
 * @brief OverlayTester::exec
 */
void OverlayTester::exec() {

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
void OverlayTester::draw_pen() {
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonPress, QPointF(191, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(190, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(188, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(186, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(184, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(182, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(181, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(179, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(177, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(175, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(173, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(170, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(166, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(164, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(162, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(159, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(157, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(155, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(152, 36), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(150, 38), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(150, 39), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 41), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 43), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 45), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 47), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(146, 50), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(146, 52), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(144, 54), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(144, 56), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(143, 59), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(143, 61), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 63), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 67), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 68), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 70), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 74), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 76), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 77), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 81), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 83), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 85), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 90), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 92), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 94), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 97), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 99), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 101), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 105), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 107), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 108), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(141, 110), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(143, 110), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(144, 112), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(146, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(148, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(150, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(150, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(152, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(153, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(157, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(159, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(161, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(162, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(164, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(166, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 114), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 112), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 110), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 107), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 105), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(168, 103), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(170, 99), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(170, 97), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 96), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 94), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 92), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(171, 90), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(173, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(175, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(177, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(179, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(182, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(184, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(186, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(190, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(191, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(193, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(197, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(199, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(200, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(202, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(206, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(208, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(210, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(213, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(215, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(217, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(220, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(224, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(226, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(229, 87), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(231, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(233, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(235, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(238, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(240, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(242, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(246, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(248, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(249, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(251, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(255, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(257, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(258, 88), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(260, 90), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(260, 92), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(262, 94), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(264, 96), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(264, 99), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 101), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 103), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 107), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 108), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 110), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 112), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 116), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 119), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 123), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 125), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 126), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 130), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 132), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 134), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 137), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 139), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 141), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 145), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 146), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 148), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 150), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 154), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 155), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 157), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(264, 161), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(262, 163), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(260, 165), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(258, 168), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(255, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(253, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(251, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(248, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(246, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(244, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(240, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(238, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(237, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(235, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(231, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(229, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(228, 170), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(226, 168), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(224, 165), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(224, 163), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(224, 161), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 157), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 155), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 154), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 150), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 148), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 146), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 145), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 141), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 139), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 137), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 134), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 132), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 130), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 126), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 125), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 123), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 119), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(222, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(224, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(228, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(229, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(231, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(235, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(237, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(238, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(240, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(244, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(246, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(248, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(251, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(253, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(255, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(258, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(260, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(262, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(266, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(267, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(269, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(273, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(275, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(277, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(278, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(282, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(284, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(286, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(289, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(291, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(293, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(296, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(298, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(300, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(304, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(305, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseMove, QPointF(307, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    main->eventFilter(main->ui->video_frame, new QMouseEvent(QEvent::MouseButtonRelease, QPointF(307, 117), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
}
