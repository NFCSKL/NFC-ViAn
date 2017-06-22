#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include "Video/shapes/zoomrectangle.h"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

enum click_tool {NONE, ZOOM, MOVE};

class FrameWidget : public QWidget
{
    Q_OBJECT
    QPainter*  painter;

    click_tool tool = NONE;
    cv::Mat current_frame;
    // Zoom
    QPoint zoom_start_pos, zoom_end_pos, prev_pos;
    bool draw_zoom_rect = false;
    bool do_zoom = false;
public:
    explicit FrameWidget(QWidget *parent = nullptr);

signals:
    void video_pressed(QPoint pos);
    void video_released(QPoint pos);
    void video_moved(QPoint pos);
    void zoom_factor(double scale_factor);
    void current_size(int width, int height);
    void moved_xy(int x, int y);
    void current_frame_size(QSize size);
public slots:
    void draw_image(cv::Mat image);
    void draw_from_playback(cv::Mat frame);
    void toggle_zoom(bool value);
protected:
    QImage _qimage;
    cv::Mat _tmp;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // FRAMEWIDGET_H
