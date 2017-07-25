#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "framemanipulator.h"

#include <QObject>
#include <QImage>
#include <QSize>

#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "zoomer.h"

class FrameProcessor : public QObject {
    Q_OBJECT
    cv::Mat m_frame;
    int m_frame_index;

    // Constants for the directions of the rotation.
    int const ROTATE_90 = 0, ROTATE_180 = 1, ROTATE_270 = 2, ROTATE_NONE = 3;
    // The limits of the rotation. This should not include the no-rotaion option.
    int const ROTATE_MIN = 0, ROTATE_MAX = 2;
    // Number of directions.
    int const ROTATE_NUM = 4;
    int m_rotate_direction = ROTATE_NONE;

    Zoomer m_zoomer;
    FrameManipulator m_manipulator;
public:
    FrameProcessor();

public slots:
    void on_new_frame(cv::Mat frame, int frame_index);
    void on_set_zoom_rect(QPoint p1, QPoint p2);
    void on_video_info(int frame_width, int frame_height, int frame_rate, int last_frame);
    void on_set_draw_area_size(QSize size);
    void on_zoom_out();
    void on_fit_screen();
    void on_rotate_right();
    void on_rotate_left();
    void on_set_bright_cont(int b_val, double c_val);
    void on_move_zoom_rect(int x, int y);
signals:
    void done_processing(cv::Mat frame, int frame_index);
private:
    void process_frame();
};

#endif // FRAMEPROCESSOR_H
