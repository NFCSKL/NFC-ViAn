#include "zoomer.h"
#include <math.h>
#include <QtDebug>

Zoomer::Zoomer() {
    anchor = QPoint(0,0);
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
    update_rect_size();
}

/**
 * @brief Zoomer::set_zoom_rect
 * Sets the zooming rectangle
 * @param p1 start point of rectangle
 * @param p2 end point
 */
void Zoomer::set_zoom_rect(QPoint p1, QPoint p2) {
    cv::Rect _tmp = cv::Rect(cv::Point(anchor.x() + p1.x() / m_scale_factor, anchor.y() + p1.y() / m_scale_factor),
                  cv::Point(anchor.x() + p2.x() / m_scale_factor, anchor.y() + p2.y() / m_scale_factor));
    cv::Rect _tmp2 = cv::Rect(cv::Point(p1.x(), p1.y()), cv::Point(p2.x(), p2.y()));
    if (_tmp2.width > 1 && _tmp2.height > 1) {
        m_zoom_rect = _tmp2;
        anchor = QPoint(m_zoom_rect.tl().x, m_zoom_rect.tl().y);
        update_scale();
        update_rect_size();
    }
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
}

/**
 * @brief Zoomer::move_zoom_rect
 * Updates the position of the zoom rectangle relative to the frame
 * @param x movement on the x-axis
 * @param y movement on the y-axis
 */
void Zoomer::move_zoom_rect(int x, int y){
    qDebug() << "i get" << x << y;
    cv::Point tl = m_zoom_rect.tl(); // top-left corner
    cv::Point br = m_zoom_rect.br(); // bottom-right corner

    int tl_x, tl_y, br_x, br_y;
    if (x < 0) {
        // Moving left
        tl_x = std::max(0, tl.x + x);
        br_x = tl_x + m_zoom_rect.width;
    } else if (x > 0) {
        // Moving right
        br_x = std::min(m_frame_size.width, br.x + x);
        tl_x = br_x - m_zoom_rect.width;
    } else {
        tl_x = tl.x;
        br_x = br.x;
    }

    if (y < 0) {
        // Moving up
        tl_y = std::max(0, tl.y + y);
        br_y = tl_y + m_zoom_rect.height;
    } else if (y > 0) {
        // Moving down
        br_y = std::min(m_frame_size.height, br.y + y);
        tl_y = br_y - m_zoom_rect.height;
    } else {
        tl_y = tl.y;
        br_y = br.y;
    }

    m_zoom_rect = cv::Rect(cv::Point(tl_x, tl_y), cv::Point(br_x, br_y));
    anchor = QPoint(tl_x, tl_y);
}

void Zoomer::center_zoom_rect(QPoint center, double zoom_step) {
    double percent_in_x_before = (center.x() - m_zoom_rect.x) / (double)(m_zoom_rect.br().x - m_zoom_rect.x);
    double percent_in_y_before = (center.y() - m_zoom_rect.y) / (double)(m_zoom_rect.br().y - m_zoom_rect.y);
    qDebug() << "percent_in before" << percent_in_x_before;
    set_scale_factor(m_scale_factor*zoom_step);
    double new_center_x = m_zoom_rect.x + percent_in_x_before*m_zoom_rect.width;
    double new_center_y = m_zoom_rect.y + percent_in_y_before*m_zoom_rect.height;
    qDebug() << "new center" << new_center_x << new_center_y;
    double diff_x = center.x() - new_center_x;
    double diff_y = center.y() - new_center_y;
    qDebug() << "diff" << diff_x << diff_y;
    move_zoom_rect(round(diff_x), round(diff_y));

//    qDebug() << "tl" << m_zoom_rect.tl().x << m_zoom_rect.tl().y;
//    qDebug() << "size" << m_zoom_rect.size().width << m_zoom_rect.size().height;
//    double percent_in = (center.x() - m_zoom_rect.x) / (double)(m_zoom_rect.br().x - m_zoom_rect.x);
//    qDebug() << "percent in" << percent_in;

//    int dif_x = center.x() - m_zoom_rect.x;
//    int dif_y = center.y() - m_zoom_rect.y;
//    qDebug() << "The difs" << dif_x << dif_y;

//    move_zoom_rect(m_zoom_rect.x, m_zoom_rect.y);


//    int current_x = (m_zoom_rect.br().x + m_zoom_rect.tl().x)/2;
//    int current_y = (m_zoom_rect.br().y + m_zoom_rect.tl().y)/2;
//    qDebug() << "current center" << current_x << current_y;
//    int x_dif = center.x() - current_x;
//    int y_dif = center.y() - current_y;
//    qDebug() << "diff" << x_dif << y_dif;
//    move_zoom_rect(x_dif, y_dif);
//    int current_x2 = (m_zoom_rect.br().x + m_zoom_rect.tl().x)/2;
//    int current_y2 = (m_zoom_rect.br().y + m_zoom_rect.tl().y)/2;
//    qDebug() << "number 2" << current_x2 << current_y2;
//    //QPoint current_center = QPoint(current_x, current_y);

//    int tl_x, tl_y, br_x, br_y;
//    tl_x = std::max(0, center.x() - m_zoom_rect.size().width/2);
//    tl_y = std::max(0, center.y() - m_zoom_rect.size().height/2);
//    br_x = std::min(m_frame_rect.br().x, center.x() + m_zoom_rect.size().width/2);
//    br_y = std::min(m_frame_rect.br().y, center.y() + m_zoom_rect.size().height/2);
//    qDebug() << "center rect" << tl_x << tl_y << br_x << br_y;
//    m_zoom_rect = cv::Rect(cv::Point(tl_x, tl_y), cv::Point(br_x, br_y));
//    anchor = QPoint(tl_x, tl_y);
//    //qDebug() << "frame_rect" << m_frame_rect.width << m_frame_rect.height;
}

/**
 * @brief Zoomer::fit_viewport
 * Adjusts the scaling factor so the frame will fit the viewport
 */
void Zoomer::fit_viewport() {
    // TODO update anchor point correctly
    m_zoom_rect = m_frame_rect;
    anchor = QPoint(0,0);
    m_scale_factor = std::min(m_viewport_size.width() / double(m_frame_size.width),
                              m_viewport_size.height() / double(m_frame_size.height));
}

void Zoomer::flip() {
    // Flips the original frame size
    set_frame_size(cv::Size(m_frame_size.height, m_frame_size.width));
    // Reset orginal state when rotating
    reset();
}

/**
 * @brief Zoomer::update_scale
 * Updates the zooming factor based on viewport and zoom rectangle sizes
 */
void Zoomer::update_scale() {
    if (m_zoom_rect.size() == cv::Size(0,0)) return;
    m_scale_factor = std::max(m_viewport_size.width() / double(m_zoom_rect.width),
                              m_viewport_size.height() / double(m_zoom_rect.height));
}

QSize Zoomer::get_viewport_size() const {
    return m_viewport_size;
}

/**
 * @brief Zoomer::update_rect_size
 * Calculates a new size for the zooming rectangle based on the scale factor
 */
void Zoomer::update_rect_size() {
    double width = m_viewport_size.width() / m_scale_factor;
    double height = m_viewport_size.height() / m_scale_factor;
    double width_diff = width - m_zoom_rect.width;
    double height_diff = height - m_zoom_rect.height;

    // Generate a new top-left and bottom-right corner for the rectangle
    // Makes sure the rectangle will not be bigger then the original frame
    cv::Point new_tl = cv::Point(std::max(0, int(m_zoom_rect.tl().x - width_diff / 2)),
                                 std::max(0, int(m_zoom_rect.tl().y - height_diff / 2)));
    cv::Point new_br = cv::Point(std::min(m_frame_rect.br().x, int(m_zoom_rect.br().x + width_diff / 2)),
                                 std::min(m_frame_rect.br().y, int(m_zoom_rect.br().y + height_diff / 2)));

    cv::Rect _tmp = cv::Rect(new_tl, new_br);
    if (_tmp.area() > 1) {
        m_zoom_rect = _tmp;
        anchor = QPoint(new_tl.x, new_tl.y);
    }
}

/**
 * @brief Zoomer::reset
 * Resets the zoom to original size
 */
void Zoomer::reset() {
    m_scale_factor = 1;
    anchor = QPoint(0,0);
    m_zoom_rect = m_frame_rect;
}

/**
 * @brief Zoomer::scale_frame
 * Scales the frame 'frame'
 * @param frame
 */
void Zoomer::scale_frame(cv::Mat &frame) {
    if (m_frame_size.width < m_zoom_rect.width || m_frame_size.height < m_zoom_rect.height) {
        qWarning("Zoom rectangle is larger then the frame. Fitting to screen");
        fit_viewport();
    }
    int interpol = m_interpol_method;
    if (m_scale_factor < 1) interpol = cv::INTER_AREA;
    cv::resize(frame(m_zoom_rect), frame, cv::Size(), m_scale_factor, m_scale_factor, interpol);
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

void Zoomer::force_bounds() {
    m_zoom_rect.height = std::min(m_frame_size.height, m_zoom_rect.height);
    m_zoom_rect.width = std::min(m_frame_size.width, m_zoom_rect.width);
}
