#ifndef ZOOMER_H
#define ZOOMER_H

#include <QPoint>
#include <QSize>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


class Zoomer{
    QSize m_viewport_size;
    QPoint anchor;
    cv::Size m_frame_size = cv::Size(0,0);
    cv::Rect m_frame_rect;
    int m_interpol_method = cv::INTER_NEAREST;
    double m_scale_factor = 1;

public:
    Zoomer();
    Zoomer(const cv::Size frame_size);
    void set_scale_factor(double scale_factor);
    void set_zoom_rect(QPoint p1, QPoint p2);
    void set_frame_size(cv::Size frame_size);
    void set_viewport_size(const QSize size);
    void move_zoom_rect(int x, int y);
    void fit_viewport();
    void flip();
    void reset();
    cv::Rect  m_zoom_rect;

    void scale_frame(cv::Mat& frame);
    cv::Rect get_zoom_rect() const;
    double get_scale_factor() const;

    QPoint get_anchor() const;
    QSize get_viewport_size() const;

    void update_rect_size();
private:
    void force_bounds();
    void update_scale();
};

#endif // ZOOMER_H
