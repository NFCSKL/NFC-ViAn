#ifndef ZOOMER_H
#define ZOOMER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>
#include <QSize>

/**
 * @brief The Zoomer class
 * the Zoomer class is used to represent a smart zoom rectangle
 * which updates it's scaling and anchor implicitly on requsts given that
 * it's settings are updated properly
 */
class Zoomer {
    // Viewport rectangle and size
    QSize m_viewport_size;
    cv::RotatedRect m_viewport;

    // Current rotation
    int m_angle{};

    // Anchor point for the top left corner of the viewport rectangle
    QPoint anchor;

    // Frame rectangle and size for the frame being resized/zoomed
    cv::Size m_original_frame_size = cv::Size(0,0);
    cv::Rect m_transformed_frame_rect;
    cv::Rect m_frame_rect;

    int m_interpol_method = cv::INTER_NEAREST;
    double m_scale_factor = 1;

public:
    Zoomer();
    Zoomer(const cv::Size frame_size);

    void area_zoom(QPoint p1, QPoint p2);
    void point_zoom(QPoint, double zoom_step);
    void update_rotation(const int& angle);
    void update_flip_state(bool h_flip, bool v_flip);
    void translate_viewport_center(int x, int y);
    void load_state(QPoint center, double scale_factor, int angle);
    void fit_viewport();
    void scale_frame(cv::Mat& frame);
    void reset();
    void enforce_frame_boundaries();

    // Setters
    void set_scale_factor(double scale_factor);
    void set_frame_size(cv::Size frame_size);
    void set_viewport_size(const QSize size);
    void set_interpolation_method(const int& method);
    void set_angle(const int& angle);

    // Getters
    int get_angle() const;
    int get_interpolation_method() const;
    double get_scale_factor() const;

    QPoint get_anchor() const;
    QPoint get_center() const;
    QSize get_viewport_size() const;
    QSize get_transformed_size() const;

    cv::RotatedRect get_viewport() const;
    cv::Rect get_frame_rect() const;
    cv::Rect get_view_rect() const;

private:
    void update_scale(const double& width, const double& height);
    void update_anchor();
    void adjust_frame_rect_rotation();
    void center();
};

#endif // ZOOMER_H
