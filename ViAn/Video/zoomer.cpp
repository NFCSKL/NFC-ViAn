#include "zoomer.h"

#include <QDebug>
#include <cmath>


const double Zoomer::DEGREES_TO_RADIANS_FACTOR = M_PI / 180;

Zoomer::Zoomer() {}

// TODO not used?
Zoomer::Zoomer(cv::Size frame_size) {
    set_frame_size(frame_size);
    reset();
}

/**
 * @brief Zoomer::enforce_frame_boundaries
 * Moves the viewport rectangle in relation to the current frame rectangle so that
 * each corner of the viewport rectangle are contained within the frame
 */
void Zoomer::enforce_frame_boundaries() {
    cv::RotatedRect temp(m_viewport.center,
                         cv::Size(m_viewport.size.width / m_scale_factor,
                                  m_viewport.size.height / m_scale_factor),
                         0);

    cv::Point2f p[4];
    temp.points(p);
    cv::Point2f tl = p[1];
    cv::Point2f br = p[3];

    double dx{}, dy{};
    if ((tl.x) < 0 && (br.x) > m_transformed_frame_rect.width) {
        dx = 0;
    } else if ((tl.x) < 0) {
        // Frame is being cropped on the left side, move center to the right
        dx = std::abs(tl.x);
    } else if ((br.x) > m_transformed_frame_rect.width) {
        // Frame is being cropped on the right side, move center to the left
        dx = m_transformed_frame_rect.width - br.x;
    }

    if ((tl.y) < 0 && (br.y) > m_transformed_frame_rect.height) {
        dy = 0;
    } else if ((tl.y) < 0) {
        // Frame is being cropped on the top, move center down
        dy = std::abs(tl.y);
    } else if ((br.y) > m_transformed_frame_rect.height) {
        // Frame is being cropped on the bottom, move center up
        dy = m_transformed_frame_rect.height - br.y;
    }
    m_viewport = cv::RotatedRect(cv::Point2f(m_viewport.center.x + std::round(dx), m_viewport.center.y + std::round(dy)),
                                 m_viewport.size,
                                 m_angle);
    update_anchor();
}

/**
 * @brief Zoomer::set_scale_factor
 * @param scale_factor
 */
void Zoomer::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;
    enforce_frame_boundaries();
}

/**
 * @brief Zoomer::set_zoom_rect
 * Zooms in at the center of the area given by the two points
 * @param p1 start point of rectangle
 * @param p2 end point
 */
void Zoomer::area_zoom(QPoint p1, QPoint p2) {
    int start_x = (p1.x() < p2.x()) ? p1.x() : p2.x();
    int end_x = (p1.x() < p2.x()) ? p2.x() : p1.x();

    int start_y = (p1.y() < p2.y()) ? p1.y() : p2.y();
    int end_y = (p1.y() < p2.y()) ? p2.y() : p1.y();

    cv::Point2f new_center(start_x + (end_x - start_x) / 2, start_y + (end_y - start_y) / 2);
    m_viewport = cv::RotatedRect(new_center, m_viewport.size, m_angle);
    update_scale(end_x - start_x, end_y - start_y);
}

/**
 * @brief Zoomer::set_frame_size
 * Sets the frame size and updates the frame rectangle
 * @param frame_size current frame size
 */
void Zoomer::set_frame_size(cv::Size frame_size) {
    m_original_frame_size = frame_size;
    m_frame_rect = cv::Rect(cv::Point(0,0), cv::Point(m_original_frame_size.width, m_original_frame_size.height));
    adjust_frame_rect_rotation();
}

/**
 * @brief Zoomer::set_viewport_size
 * Updates the size of the viewport rectangle
 * @param size current size of the viewport
 */
void Zoomer::set_viewport_size(const QSize size) {
    m_viewport_size = size;
    m_viewport = cv::RotatedRect(m_viewport.center,
                                 cv::Point2f(size.width(),size.height()),
                                 m_angle);
    enforce_frame_boundaries();
}

/**
 * @brief Zoomer::set_interpolation_method
 * Sets the interpolation method
 * @param method
 */
void Zoomer::set_interpolation_method(const int &method) {
    m_interpol_method = method;
}

/**
 * @brief Zoomer::set_angle
 * Sets the angle in degrees (does not update any of the necessary settings)
 * @param angle
 */
void Zoomer::set_angle(const int &angle) {
    m_angle = angle;
}

/**
 * @brief Zoomer::update_rotation
 * Rotates the viewport rectangle around its center point by
 * the angle difference between the given and current angle.
 *
 * Also updates the center position of the viewport rectangle,
 * since it will change when the uncut frame is rotated.
 *
 * Finally it rotates the frame rectangle if necessary
 * @param angle :   Desired rotation
 */
void Zoomer::update_rotation(const int &angle) {
    double angle_diff{(angle - m_angle) * DEGREES_TO_RADIANS_FACTOR};

    // Translate by negative pivot of old frame size
    double translated_x{m_viewport.center.x - m_transformed_frame_rect.width / 2};
    double translated_y{m_viewport.center.y - m_transformed_frame_rect.height / 2};

    // Rotate around pivot
    double rotated_x{translated_x * std::cos(angle_diff) - translated_y * std::sin(angle_diff)};
    double rotated_y{translated_x * std::sin(angle_diff) + translated_y * std::cos(angle_diff)};

    m_angle = angle;
    adjust_frame_rect_rotation();

    // Translate back using the new frame size
    translated_x = rotated_x + m_transformed_frame_rect.width / 2;
    translated_y = rotated_y + m_transformed_frame_rect.height / 2;
    cv::Point2f rotated_center(translated_x, translated_y);
    m_viewport = cv::RotatedRect(rotated_center, m_viewport.size, angle);
    update_anchor();
}

/**
 * @brief Zoomer::translate_viewport center
 * Updates the viewport rectangles position in relation to the frame rectangle
 * @param x movement on the x-axis
 * @param y movement on the y-axis
 */
void Zoomer::translate_viewport_center(int x, int y){
    // TODO don't move outside frame rect
    bool is_moving_left{x < 0}, is_moving_right{x > 0}, is_moving_up{y < 0}, is_moving_down{y > 0};

    cv::RotatedRect contained_rect(cv::Point2f(m_viewport.center.x + x, m_viewport.center.y + y),
                                  cv::Size(m_viewport.size.width / m_scale_factor, m_viewport.size.height / m_scale_factor),
                                  0);


    int length{4};
    cv::Point2f p[length];
    contained_rect.points(p); //bl, tl, tr, br

    bool left_contained{p[0].x >= 0 || p[1].x >= 0};
    bool up_contained{p[1].y >= 0 || p[2].y >= 0};
    bool right_contained{p[2].x <= m_transformed_frame_rect.width || p[3].x <= m_transformed_frame_rect.width};
    bool down_contained{p[3].y <= m_transformed_frame_rect.height || p[0].y <= m_transformed_frame_rect.height};

    int dx{}, dy{};
    if ((is_moving_left && left_contained) || (is_moving_right && right_contained)) {
        dx = x;
    }
    if ((is_moving_up && up_contained) || (is_moving_down && down_contained)) {
        dy = y;
    }

    m_viewport = cv::RotatedRect(cv::Point2f(m_viewport.center.x + dx, m_viewport.center.y + dy),
                                 m_viewport.size,
                                 m_angle);
    update_anchor();
}

/**
 * @brief Zoomer::point_zoom
 * Modifies the zoom level and translates the viewport center
 * so that the given point will stay in the same place on screen.
 * @param center_p  :   Point to zoom at (unzoomed coordinates)
 * @param zoom_step :   How much the zoom should be modified
 */
void Zoomer::point_zoom(QPoint original_point, double zoom_step) {
    // Calculate point displacement relative the old viewport center
    double dx = (original_point.x() - m_viewport.center.x) * (zoom_step - 1);
    double dy = (original_point.y() - m_viewport.center.y) * (zoom_step - 1);

    // Translate center
    m_viewport = cv::RotatedRect(cv::Point2f(m_viewport.center.x + dx, m_viewport.center.y + dy),
                                 m_viewport.size,
                                 m_angle);
    set_scale_factor(m_scale_factor * zoom_step);
}

/**
 * @brief Zoomer::set_state
 * Updates the zoom state
 * @param center_point :    center point of the viewport (coordinates should be in relation to the angle)
 * @param scale_factor :    zoom level
 * @param angle        :    image rotation
 */
void Zoomer::load_state(QPoint center_point, double scale_factor, int angle) {
    set_scale_factor(scale_factor);
    m_angle = angle;
    adjust_frame_rect_rotation();

    // Check for default state
    if (center_point.x() == -1 && center_point.y() == -1) {
        if (scale_factor == 1.) {
            fit_viewport();
        } else {
            center();
        }
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
    m_scale_factor = std::min(m_viewport_size.width() / double(m_transformed_frame_rect.width),
                              m_viewport_size.height() / double(m_transformed_frame_rect.height));
    center();
}

/**
 * @brief Zoomer::update_scale
 * Updates the zooming factor based on viewport and zoom rectangle sizes
 */
void Zoomer::update_scale(const double& width, const double& height) {
    if (width * height == 0.) return;
    set_scale_factor(std::min(m_viewport_size.width() / width,
                              m_viewport_size.height() / height));
}

/**
 * @brief Zoomer::update_anchor
 * Updates the anchor point, i.e. the top left corner of the intersection rectangle
 * between the viewport rectangle and the frame rectangle
 */
void Zoomer::update_anchor() {
    anchor = QPoint(get_view_rect().tl().x, get_view_rect().y);
}

/**
 * @brief Zoomer::get_viewport_size
 * @return QSize    :   The size of the viewport
 */
QSize Zoomer::get_viewport_size() const {
    return m_viewport_size;
}

QSize Zoomer::get_transformed_size() const {
    return QSize(m_transformed_frame_rect.width, m_transformed_frame_rect.height);
}

/**
 * @brief Zoomer::get_viewport
 * @return cv::RotatedRect  :   Viewport rectangle
 */
cv::RotatedRect Zoomer::get_viewport() const {
    return m_viewport;
}

/**
 * @brief Zoomer::get_angle
 * @return int current rotation angle
 */
int Zoomer::get_angle() const {
    return m_angle;
}

/**
 * @brief Zoomer::get_frame_rect
 * @return cv::Rect :   frame rectangle
 */
cv::Rect Zoomer::get_frame_rect() const {
    return m_frame_rect;
}

/**
 * @brief Zoomer::get_view_rect
 * Calculates the intersection rectangle between
 * the scaled viewport rectangle and the frame rectangle
 * @return cv::Rect :   intersection rectangle
 */
cv::Rect Zoomer::get_view_rect() const {
    cv::RotatedRect scaled_viewport(m_viewport.center,
                                    cv::Size(m_viewport.size.width / m_scale_factor,
                                             m_viewport.size.height / m_scale_factor),
                                    0);
    return scaled_viewport.boundingRect() & m_transformed_frame_rect;
}

/**
 * @brief Zoomer::get_interpolation_method
 * @return int  :   current interpolation method
 */
int Zoomer::get_interpolation_method() const {
    return m_interpol_method;
}

/**
 * @brief Zoomer::reset
 * Resets rotation angle and zoom level.
 * Also centers the viewport to the frame
 */
void Zoomer::reset() {
    m_scale_factor = 1;
    center();
}

/**
 * @brief Zoomer::center
 * Centers the viewport rectangle on the frame
 */
void Zoomer::center() {
    int center_x{m_transformed_frame_rect.tl().x + m_transformed_frame_rect.br().x / 2}, center_y{m_transformed_frame_rect.tl().y + m_transformed_frame_rect.br().y / 2};
    m_viewport = cv::RotatedRect(cv::Point2f(center_x, center_y), m_viewport.size, m_angle);
    update_anchor();
}

/**
 * @brief Zoomer::flip
 * Flips the frame rectangle 90 degrees
 */
void Zoomer::adjust_frame_rect_rotation() {
    if (m_angle == 90 || m_angle == 270) {
        m_transformed_frame_rect = cv::Rect(cv::Point(0,0), cv::Point(m_frame_rect.height, m_frame_rect.width));
    } else {
        m_transformed_frame_rect = m_frame_rect;
    }
}

/**
 * @brief Zoomer::scale_frame
 * "Extracts" the area given by the intersection between the
 *  viewport and frame rectangle and scales it by the current scale factor.
 * @param frame
 */
void Zoomer::scale_frame(cv::Mat &frame) {
    cv::Rect view_rectangle = get_view_rect();
    // TODO assert that the view_rectangle has an area

    int interpol = m_interpol_method;
    if (m_scale_factor < 1) interpol = cv::INTER_AREA;

    try {
        cv::resize(frame(view_rectangle), frame, cv::Size(), m_scale_factor, m_scale_factor, interpol);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        qCritical() << "Exception: " << err_msg;
        qWarning() << "Failed to resize frame to view rectangle. Resetting zoomer";
        center();
    }
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
