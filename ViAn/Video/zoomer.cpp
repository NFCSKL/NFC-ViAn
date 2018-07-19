#include "zoomer.h"
#include <math.h>
#include <QDebug>

Zoomer::Zoomer() {
//    anchor = QPoint(0,0);
}

Zoomer::Zoomer(cv::Size frame_size) {
    set_frame_size(frame_size);
    reset();
}

/**
 * @brief Zoomer::set_scale_factor
 * Sets the scaling factor and also updates the zooming rectangle
 * @param scale_factor
 */
void Zoomer::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;
    update_anchor();
}

/**
 * @brief Zoomer::set_zoom_rect
 * Sets the zooming rectangle
 * @param p1 start point of rectangle
 * @param p2 end point
 */
void Zoomer::set_zoom_rect(QPoint p1, QPoint p2) {
    cv::Point2f new_center(p1.x() + (p2.x() - p1.x()) / 2, p1.y() + (p2.y() - p1.y()) / 2);
    m_viewport = cv::RotatedRect(new_center,
                                 m_viewport.size,
                                 m_angle);

    update_scale(p2.x() - p1.x(), p2.y() - p1.y());
    update_anchor();
}

/**
 * @brief Zoomer::set_frame_size
 * Sets the frame size and updates the frame rectangle
 * @param frame_size current frame size
 */
void Zoomer::set_frame_size(cv::Size frame_size) {
    m_frame_size = frame_size;
    m_frame_rect = cv::Rect(cv::Point(0,0), cv::Point(m_frame_size.width, m_frame_size.height));
}

/**
 * @brief Zoomer::set_viewport_size
 * Sets the viewport size variable
 * @param size current size of the viewport
 */
void Zoomer::set_viewport_size(const QSize size) {
    m_viewport_size = size;
    m_viewport = cv::RotatedRect(m_viewport.center,
                                 cv::Point2f(size.width(),size.height()),
                                 m_angle);
    update_anchor();
}

/**
 * @brief Zoomer::set_interpolation_method
 * Sets the interpolation method to be used
 * @param method
 */
void Zoomer::set_interpolation_method(const int &method) {
    m_interpol_method = method;
}

void Zoomer::update_rotation(const int &angle) {
    double angle_diff{(angle - m_angle) * M_PI /180};
    // Translate by negative pivot of old frame size
    double translated_x{m_viewport.center.x - m_frame_rect.width / 2};
    double translated_y{m_viewport.center.y - m_frame_rect.height / 2};

    // Rotate around pivot
    double rotated_x{translated_x * std::cos(angle_diff) - translated_y * std::sin(angle_diff)};
    double rotated_y{translated_x * std::sin(angle_diff) + translated_y * std::cos(angle_diff)};

    // Translate back using the new frame size
    translated_x = rotated_x + m_frame_rect.height / 2;
    translated_y = rotated_y + m_frame_rect.width / 2;
    cv::Point2f rotated_center(translated_x, translated_y);
    m_viewport = cv::RotatedRect(rotated_center, m_viewport.size, angle);

    // Rotate frame rectangle if necessary
    if (std::abs(angle - m_angle) == 90 || std::abs(angle - m_angle) == 270) {
        flip();
    }
    update_anchor();
    m_angle = angle;
}

/**
 * @brief Zoomer::move_zoom_rect
 * Updates the position of the zoom rectangle relative to the frame
 * @param x movement on the x-axis
 * @param y movement on the y-axis
 */
void Zoomer::move_viewport_center(int x, int y){
    // TODO don't move outside frame rect
    bool is_moving_left{x < 0}, is_moving_right{x > 0}, is_moving_up{y < 0}, is_moving_down{y > 0};
    cv::Point tl = m_zoom_rect.tl(); // top-left corner
    cv::Point br = m_zoom_rect.br(); // bottom-right corner

    cv::Point2f center = m_viewport.center;
    cv::Point2f moved_center = center;
    cv::RotatedRect contained_rect(cv::Point2f(center.x + x, center.y + y),
                                  m_viewport.size, 0);


    int length{4};
    cv::Point2f p[length];
    contained_rect.points(p); //bl, tl, tr, br
    cv::Rect intersecting_rect = cv::Rect(p[1], p[3]) & m_frame_rect;
    if (intersecting_rect.area() > 0) {
        m_viewport = contained_rect;
    }

    int tl_x, tl_y, br_x, br_y;
    if (is_moving_left) {
        // Moving left
        tl_x = std::max(0, tl.x + x);
        br_x = tl_x + m_zoom_rect.width;
        if (m_frame_rect.contains(p[0]) || m_frame_rect.contains(p[1])) {
            moved_center = cv::Point2f(moved_center.x + x, moved_center.y);
        }
    } else if (is_moving_right) {
        // Moving right
        br_x = std::min(m_frame_size.width, br.x + x);
        tl_x = br_x - m_zoom_rect.width;
        if (m_frame_rect.contains(p[2]) && m_frame_rect.contains(p[3])) {
            moved_center = cv::Point2f(moved_center.x + x, moved_center.y);
        }
    } else {
        tl_x = tl.x;
        br_x = br.x;
    }

    if (is_moving_up) {
        // Moving up
        tl_y = std::max(0, tl.y + y);
        br_y = tl_y + m_zoom_rect.height;
        if (m_frame_rect.contains(p[1]) || m_frame_rect.contains(p[2])) {
            moved_center = cv::Point2f(moved_center.x, moved_center.y + y);
        }
    } else if (is_moving_down) {
        // Moving down
        br_y = std::min(m_frame_size.height, br.y + y);
        tl_y = br_y - m_zoom_rect.height;
        if (m_frame_rect.contains(p[0]) || m_frame_rect.contains(p[3])) {
            moved_center = cv::Point2f(moved_center.x, moved_center.y + y);
        }
    } else {
        tl_y = tl.y;
        br_y = br.y;
    }
    update_anchor();
}

/**
 * @brief Zoomer::point_zoom
 * Centers the viewport to the given point and modifies the zoom level
 * @param center_p  :   Point to move viewport to
 * @param zoom_step :   How much the zoom should be modified
 */
void Zoomer::point_zoom(QPoint original_point, double zoom_step) {
    qDebug() << "center_zoom_rect()";
    int {old_distance_from_center}
    QPoint scaled_point = original_point / zoom_step;
    int x_distance{original_point.x() - scaled_point.x()};
    int y_distance{original_point.y() - scaled_point.y()};
    qDebug() << "Original point: " << original_point;
    qDebug() << "Scaled point: " << scaled_point;
    qDebug() << "Old center: " << QPoint(m_viewport.center.x, m_viewport.center.y);
    qDebug() << "New center: " << QPoint(m_viewport.center.x - x_distance, m_viewport.center.y - y_distance);
//    double percent_in_x_before = (center.x() - m_zoom_rect.x) / (double)(m_zoom_rect.br().x - m_zoom_rect.x);
//    double percent_in_y_before = (center.y() - m_zoom_rect.y) / (double)(m_zoom_rect.br().y - m_zoom_rect.y);
    set_scale_factor(m_scale_factor * zoom_step);
    m_viewport = cv::RotatedRect(cv::Point2f(m_viewport.center.x + x_distance, m_viewport.center.y + y_distance),
                                 m_viewport.size,
                                 m_angle);
    update_anchor();
//    double new_center_x = m_zoom_rect.x + percent_in_x_before*m_zoom_rect.width;
//    double new_center_y = m_zoom_rect.y + percent_in_y_before*m_zoom_rect.height;
//    double diff_x = center.x() - new_center_x;
//    double diff_y = center.y() - new_center_y;
//    move_viewport_center(round(diff_x), round(diff_y));
}

/**
 * @brief Zoomer::set_state
 * Set the zoomers state, which means set the scale factor and the zoom rect
 * @param anchor
 * @param scale_factor
 */
void Zoomer::set_state(QPoint center_point, double scale_factor, int angle) {
    set_scale_factor(scale_factor);

    if (std::abs(angle - m_angle) == 90 || std::abs(angle - m_angle) == 270) {
        flip();
    }
    m_angle = angle;

    if (center_point.x() == -1 && center_point.y() == -1) {
        center();
    } else {
        m_viewport = cv::RotatedRect(cv::Point2f(center_point.x(), center_point.y()),
                                     m_viewport.size,
                                     m_angle);
        update_anchor();
    }
}

/**
 * @brief Zoomer::fit_viewport
 * Adjusts the scaling factor so the frame will fit the viewport
 */
void Zoomer::fit_viewport() {
    m_zoom_rect = m_frame_rect;
    m_scale_factor = std::min(m_viewport_size.width() / double(m_frame_size.width),
                              m_viewport_size.height() / double(m_frame_size.height));
    center();
}

/**
 * @brief Zoomer::update_scale
 * Updates the zooming factor based on viewport and zoom rectangle sizes
 */
void Zoomer::update_scale(const double& width, const double& height) {
    if (m_zoom_rect.size() == cv::Size(0,0)) return;
    m_scale_factor = std::max(m_viewport_size.width() / width,
                              m_viewport_size.height() / height);
}

/**
 * @brief Zoomer::update_anchor
 * Updates the anchor point, i.e. the top left corner of the intersection rectangle
 * between the viewport rectangle and the frame rectangle
 */
void Zoomer::update_anchor() {
    anchor = QPoint(get_cutting_rect().tl().x, get_cutting_rect().y);
}

QSize Zoomer::get_viewport_size() const {
    return m_viewport_size;
}

cv::RotatedRect Zoomer::get_viewport() const {
    return m_viewport;
}

int Zoomer::get_angle() const {
    return m_angle;
}

cv::Rect Zoomer::get_frame_rect() const {
    return m_frame_rect;
}

cv::Rect Zoomer::get_cutting_rect() const {
    cv::RotatedRect scaled_viewport(m_viewport.center,
                                    cv::Size(m_viewport.size.width / m_scale_factor,
                                             m_viewport.size.height / m_scale_factor),
                                    0);
    return scaled_viewport.boundingRect() & m_frame_rect;
}

/**
 * @brief Zoomer::get_interpolation_method
 * @return current interpolation method
 */
int Zoomer::get_interpolation_method() const {
    return m_interpol_method;
}

/**
 * @brief Zoomer::reset
 * Resets the zoom to original size
 */
void Zoomer::reset() {
    m_angle = 0;
    m_scale_factor = 1;
    center();
}

/**
 * @brief Zoomer::center
 * Centers the viewport rect on the frame
 */
void Zoomer::center() {
    int center_x{m_frame_rect.tl().x + m_frame_rect.br().x / 2}, center_y{m_frame_rect.tl().y + m_frame_rect.br().y / 2};
    m_viewport = cv::RotatedRect(cv::Point2f(center_x, center_y), m_viewport.size, m_angle);
    update_anchor();
}

/**
 * @brief Zoomer::flip
 * Flips the frame rectangle 90 degrees
 */
void Zoomer::flip() {
    set_frame_size(cv::Size(m_frame_size.height, m_frame_size.width));
}

/**
 * @brief Zoomer::scale_frame
 * Scales the frame 'frame'
 * @param frame
 */
void Zoomer::scale_frame(cv::Mat &frame) {
    qDebug() << "\n---------------------------------\nZooming frame";
    qDebug() << "Stored frame size: " << m_frame_size.width << "x" << m_frame_size.height;
    qDebug() << "Actual frame size: " << frame.cols << "x" << frame.rows;
    qDebug() << "Viewport size: " << m_viewport.size.width << "x" << m_viewport.size.height;
    qDebug() << "Viewport center: " << m_viewport.center.x << ", " << m_viewport.center.y;
    qDebug() << "Cutting rect size: " << get_cutting_rect().width << "x" << get_cutting_rect().height;
    qDebug() << "Frame rotation: " << m_angle;
    qDebug() << "Anchor: " << anchor;

    int interpol = m_interpol_method;
    if (m_scale_factor < 1) interpol = cv::INTER_AREA;

    try {
        cv::resize(frame(get_cutting_rect()), frame, cv::Size(), m_scale_factor, m_scale_factor, interpol);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        qDebug() << "Failed to resize frame";
        qCritical() << "Exception: " << err_msg;
    }
}

/**
 * @brief Zoomer::get_zoom_rect
 * @return cv::Rect zooming rectangle
 */
cv::Rect Zoomer::get_zoom_rect() const {
    return m_zoom_rect;
}

/**
 * @brief Zoomer::get_scale_factor
 * @return double scale factor
 */
double Zoomer::get_scale_factor() const {
    return m_scale_factor;
}

QPoint Zoomer::get_anchor() const {
    return anchor;
}

QPoint Zoomer::get_center() const {
    return QPoint(m_viewport.center.x, m_viewport.center.y);
}

// TODO unused
void Zoomer::force_bounds() {
    m_zoom_rect.height = std::min(m_frame_size.height, m_zoom_rect.height);
    m_zoom_rect.width = std::min(m_frame_size.width, m_zoom_rect.width);
}
