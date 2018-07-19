#ifndef ZOOMER_H
#define ZOOMER_H

#include <QPoint>
#include <QSize>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

/**
 * @brief The Zoomer class
 * the Zoomer class is used to represent a smart zoom rectangle
 * which updates it's scaling and anchor implicitly on requsts given that
 * it's settings are updated properly
 */
class Zoomer{
    // Viewport rectangle and size
    QSize m_viewport_size;
    cv::RotatedRect m_viewport;
    // Current rotation
    int m_angle{};

    // Anchor point for the top left corner of the viewport rectangle
    QPoint anchor;

    // Frame rectangle and size for the frame being resized/zoomed
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
    void set_interpolation_method(const int& method);
    void update_rotation(const int& angle);

    void move_viewport_center(int x, int y);
    void point_zoom(QPoint, double zoom_step);
    void set_state(QPoint anchor, double scale_factor, int angle);
    void fit_viewport();
    void reset();
    void center();
    void flip();
    cv::Rect m_zoom_rect;

    void scale_frame(cv::Mat& frame);
    cv::Rect get_zoom_rect() const;
    double get_scale_factor() const;

    QPoint get_anchor() const;
    QPoint get_center() const;

    QSize get_viewport_size() const;
    cv::RotatedRect get_viewport() const;

    int get_angle() const;
    int get_interpolation_method() const;
    cv::Rect get_frame_rect() const;
    cv::Rect get_cutting_rect() const;

    void update_rect_size();
private:
    void force_bounds();
    void update_scale(const double& width, const double& height);
    void update_anchor();
};

#endif // ZOOMER_H
