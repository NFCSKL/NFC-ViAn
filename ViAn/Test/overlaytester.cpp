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
    std::cout << "Integration test: Overlay.\n";
    std::cout << "Running integration test...\n";

    // Setup video player
    main->mvideo_player->capture.open("seq_01.mp4");
    main->mvideo_player->frame_rate = main->mvideo_player->capture.get(CV_CAP_PROP_FPS);
    main->mvideo_player->num_frames = main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_COUNT);
    main->mvideo_player->zoom_area->set_size(main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_WIDTH), main->mvideo_player->capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    main->set_pause_video();
    main->mvideo_player->frame_width = main->mvideo_player->capture.get(CV_CAP_PROP_FRAME_WIDTH);
    main->mvideo_player->frame_height = main->mvideo_player->capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    main->set_playback_frame(11);
    main->on_action_show_hide_overlay_triggered();

    // Draw
    QVector<QMouseEvent*> event_list;
    event_list.append(new QMouseEvent(QEvent::MouseButtonPress, QPointF(10,10), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    event_list.append(new QMouseEvent(QEvent::MouseMove, QPointF(100,100), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    event_list.append(new QMouseEvent(QEvent::MouseButtonRelease, QPointF(100,200), Qt::LeftButton, Qt::LeftButton, Qt::ControlModifier));
    for (QMouseEvent* ev : event_list) {
        main->eventFilter(main->ui->video_frame, ev);
    }

    // Export a bookmark
    int frame_number = main->mvideo_player->get_current_frame_num();
    QImage frame = main->mvideo_player->get_current_frame_unscaled();
    Bookmark* bookmark = new Bookmark(frame_number, frame, "integration_test1", "Bookmark Text");
    bookmark->export_frame();

    std::cout << "Integration test done.\n";
}
