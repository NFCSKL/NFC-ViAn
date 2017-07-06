#include "framewidget.h"
#include <QDebug>
#include "Project/Analysis/analysis.h"

FrameWidget::FrameWidget(QWidget *parent) : QWidget(parent) {
}

void FrameWidget::draw_from_playback(cv::Mat frame) {
    current_frame = frame;
    draw_image(frame);
}

void FrameWidget::toggle_zoom(bool value) {
    if (value) {
        tool = ZOOM;
        setCursor(Qt::CrossCursor);
    } else {
        unsetCursor();
        tool = NONE;
    }
}

void FrameWidget::set_scroll_area_size(QSize size) {
    m_scroll_area_size = size;
}

void FrameWidget::set_analysis(Analysis* analysis) {
    m_analysis = analysis;
}

/**
 * @brief FrameWidget::clear_analysis
 * Forgets the current analysis
 */
void FrameWidget::clear_analysis() {
    m_analysis = nullptr;
    ooi_rects.clear();
}

/**
 * @brief FrameWidget::set_detections_on_frame
 * Updates the oois_rects with all detections to be painted
 * @param frame_num
 */
void FrameWidget::set_detections_on_frame(int frame_num) {
    if (m_analysis != nullptr) {
        ooi_rects = m_analysis->get_detections_on_frame(frame_num);
        repaint();
    }
}

void FrameWidget::set_detections(bool detections) {
    m_detections = detections;
    repaint();  //Repaint to update the current frame directly
}

void FrameWidget::draw_image(cv::Mat image) {
    // Convert the image to the RGB888 format
    switch (image.type()) {
        case CV_8UC1:
            cvtColor(image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(image, _tmp, CV_BGR2RGB);
            break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    this->setFixedSize(image.cols, image.rows);
    repaint();
}

cv::Mat FrameWidget::get_mat() const {
    return _tmp.clone();
}

/**
 * @brief FrameWidget::paintEvent
 * @param event
 */
void FrameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);

    if (draw_zoom_rect) {


        QPoint start = zoom_start_pos;
        QPoint end = zoom_end_pos;

        int width = end.x() - start.x();
        int height = end.y() - start.y();

        double width_ratio = double(width) / current_frame.cols;
        double height_mod = std::copysign(current_frame.rows * width_ratio, height);

        end = QPoint(end.x(), start.y() + height_mod);

        painter.setPen(QColor(255,0,0));
        QRectF tmp(start, end);
        painter.drawRect(tmp);


        painter.setPen(QColor(0,255,0));
        QRectF zoom(zoom_start_pos, zoom_end_pos);
        painter.drawRect(zoom);
    }
    if (m_detections) {
        for (cv::Rect rect : ooi_rects) {
            QPoint tl(rect.x, rect.y);
            QPoint br(rect.x+rect.width, rect.y+rect.height);

            QRectF detect_rect(tl, br);
            painter.setPen(QColor(0,0,255));
            painter.drawRect(detect_rect);
        }
    }

    painter.end();
}

/**
 * @brief FrameWidget::resizeEvent
 * @param event
 */
void FrameWidget::resizeEvent(QResizeEvent *event) {
    emit current_size(width(), height());
}

/**
 * @brief FrameWidget::mousePressEvent
 * @param event
 */
void FrameWidget::mousePressEvent(QMouseEvent *event) {
    switch (tool) {
    case ZOOM:
        if (event->button() == Qt::RightButton) {
            init_panning(event->pos());
        } else if (event->button() == Qt::LeftButton) {
            if (event->modifiers() == Qt::ControlModifier) {
                do_zoom_out = true;
            } else {
                init_zoom(event->pos());
            }
        }
        break;
    default:
        break;
    }
}

/**
 * @brief FrameWidget::mouseReleaseEvent
 * @param event
 */
void FrameWidget::mouseReleaseEvent(QMouseEvent *event) {
    switch (tool) {
    case ZOOM: {
        if (event->button() == Qt::RightButton) {
            end_panning();
        } else if (event->button() == Qt::LeftButton){
            if (do_zoom_out) {
                emit trigger_zoom_out();
                do_zoom_out = false;
            } else {
                end_zoom();
            }
        }
        break;
    }
    default:
        break;
    }
}

/**
 * @brief FrameWidget::mouseMoveEvent
 * @param event
 */
void FrameWidget::mouseMoveEvent(QMouseEvent *event) {
    switch (tool) {
    case ZOOM:
        if (event->buttons() == Qt::RightButton){
            panning(event->pos());
        } else if (event->buttons() == Qt::LeftButton && !do_zoom_out) {
            zoom(event->pos());
        }
        break;
    default:
        break;
    }
}

/**
 * @brief FrameWidget::init_panning
 * Init panning on frame
 * @param pos
 */
void FrameWidget::init_panning(QPoint pos) {
    prev_pos = pos;
    setCursor(Qt::ClosedHandCursor);
}

/**
 * @brief FrameWidget::init_zoom
 * Init zoom on frame
 * @param pos
 */
void FrameWidget::init_zoom(QPoint pos) {
    zoom_start_pos = pos;
}

/**
 * @brief FrameWidget::panning
 * Is panning. Emits the new pos on frame.
 * @param pos
 */
void FrameWidget::panning(QPoint pos) {
    // Using panning
    QPoint _tmp = prev_pos - pos;
    emit moved_xy(_tmp.x(), _tmp.y());
    prev_pos = pos;
}

/**
 * @brief FrameWidget::zoom
 * Updates and redraws the zooming rect
 * @param pos
 */
void FrameWidget::zoom(QPoint pos) {
    zoom_end_pos = pos;
    draw_zoom_rect = true;
    repaint();
}

/**
 * @brief FrameWidget::end_panning
 * Panning stopped
 */
void FrameWidget::end_panning() {
    setCursor(Qt::CrossCursor);
}

/**
 * @brief FrameWidget::end_zoom
 * Emits the points of the zooming rect
 */
void FrameWidget::end_zoom() {
    draw_zoom_rect = false;
    repaint();

    // Scale factor
    int width = std::abs(zoom_start_pos.x() - zoom_end_pos.x());
    int height = std::abs(zoom_start_pos.y() - zoom_end_pos.y());
    double width_ratio = current_frame.cols / double(width );
    double height_ratio = current_frame.rows / double(height);

    // ROI rect points
    int wid = zoom_end_pos.x() - zoom_start_pos.x();
    int hei = zoom_end_pos.y() - zoom_start_pos.y();

    double wid_ratio = double(wid) / current_frame.cols;
    double height_mod = std::copysign(current_frame.rows * wid_ratio, hei);
    QPoint end = QPoint(zoom_end_pos.x(), zoom_start_pos.y() + height_mod);

    cv::Rect zoom_rect(cv::Point(zoom_start_pos.x(), zoom_start_pos.y()), cv::Point(end.x(), end.y()));
    double  scale_ratio = std::min(m_scroll_area_size.width() / double(zoom_rect.width), m_scroll_area_size.height() / double(zoom_rect.height));

    emit zoom_points(zoom_start_pos, end);
}




