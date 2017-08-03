#include "framewidget.h"
#include <QDebug>
#include "Project/Analysis/analysis.h"
#include <QTime>
#include <QThread>

FrameWidget::FrameWidget(QWidget *parent) : QWidget(parent) {}

void FrameWidget::toggle_zoom(bool value) {
    if (value) {
        tool = ZOOM;
        setCursor(Qt::CrossCursor);
    } else {
        unsetCursor();
        tool = NONE;
    }
}

void FrameWidget::set_analysis_tool()
{
    tool = ANALYSIS_BOX;
}


void FrameWidget::set_scroll_area_size(QSize size) {
    m_scroll_area_size = size;
}

void FrameWidget::set_analysis(AnalysisProxy *analysis) {
    m_analysis = analysis->load_analysis();
}

/**
 * @brief FrameWidget::clear_analysis
 * Forgets the current analysis
 */
void FrameWidget::clear_analysis() {
    m_analysis = nullptr;
}

/**
 * @brief FrameWidget::set_detections_on_frame
 * Updates the oois_rects with all detections to be painted
 * @param frame_num
 */
void FrameWidget::set_detections_on_frame(int frame_num) {
    if (m_analysis != nullptr) {
        ooi_rects = m_analysis->get_detections_on_frame(frame_num);
    }
}

void FrameWidget::set_detections(bool detections) {
    m_detections = detections;
}

void FrameWidget::set_show_detections(bool show) {
    show_detections = show;
}

void FrameWidget::set_anchor(QPoint p) {
    anchor = p;
}

void FrameWidget::update(){
    repaint();
}

cv::Mat FrameWidget::get_mat() const {
    return _tmp_frame.clone();
}

void FrameWidget::on_new_image(cv::Mat frame, int frame_index) {
    current_frame = frame;
    switch (frame.type()) {
        case CV_8UC1:
            cvtColor(frame, _tmp_frame, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(frame, _tmp_frame, CV_BGR2RGB);
            break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(frame.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp_frame.data, _tmp_frame.cols, _tmp_frame.rows, _tmp_frame.cols*3, QImage::Format_RGB888);
    setFixedSize(_qimage.size());
    set_detections_on_frame(frame_index);
    repaint();
}

/**
 * @brief FrameWidget::paintEvent
 * @param event
 */
void FrameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);

    if (mark_rect) {

        if(tool == ZOOM){
            QPoint start = rect_start;
            QPoint end = rect_end;


            int width = end.x() - start.x();
            int height = end.y() - start.y();

            double width_ratio = double(width) / _qimage.width();
            double height_mod = std::copysign(_qimage.height() * width_ratio, height);

            end = QPoint(end.x(), start.y() + height_mod);

            painter.setPen(QColor(255,0,0));
            QRectF tmp(start, end);
            painter.drawRect(tmp);
        }
        painter.setPen(QColor(0,255,0));
        QRectF zoom(rect_start, rect_end);
        painter.drawRect(zoom);
    }
    if (m_detections && show_detections) {
        for (cv::Rect rect : ooi_rects) {
            QPoint tl(rect.x, rect.y);
            QPoint br(rect.x+rect.width, rect.y+rect.height);
            QRectF detect_rect((tl-anchor)*m_scale_factor, (br-anchor)*m_scale_factor);
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
    case ANALYSIS_BOX:
        set_rect_start(event->pos());
        break;
    case ZOOM:
        if (event->button() == Qt::RightButton) {
            init_panning(event->pos());
        } else if (event->button() == Qt::LeftButton) {
            if (event->modifiers() == Qt::ControlModifier) {
                do_zoom_out = true;
            } else {
                set_rect_start(event->pos());
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
    case ZOOM:
    {
        if (event->button() == Qt::RightButton) {
            end_panning();
        } else if (event->button() == Qt::LeftButton){
            if (do_zoom_out) {
                emit trigger_zoom_out();
                do_zoom_out = false;
            }else {
                end_zoom();
            }
        }
        break;
    }
    case ANALYSIS_BOX:
    {
        AnalysisSettings* settings = new AnalysisSettings(MOTION_DETECTION);
        settings->setBounding_box(cv::Rect(rect_start.x(), rect_start.y(),
                                           rect_end.x(),rect_end.y()));
        emit quick_analysis(settings);
        qDebug () << "TODO:analysis rect needs scaling";
        tool = NONE;
        mark_rect = false;
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
    case ANALYSIS_BOX:
    case ZOOM:
        if (event->buttons() == Qt::RightButton){
            panning(event->pos());
        } else if (event->buttons() == Qt::LeftButton && !do_zoom_out) {
            rect_update(event->pos());
        }
        break;
    default:
        break;
    }
}

void FrameWidget::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;
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
void FrameWidget::set_rect_start(QPoint pos) {
    rect_start = pos;
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
void FrameWidget::rect_update(QPoint pos) {
    rect_end = pos;
    mark_rect = true;
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
    mark_rect = false;
    repaint();

    // Scale factor
    int width = std::abs(rect_start.x() - rect_end.x());
    int height = std::abs(rect_start.y() - rect_end.y());
    double width_ratio = _qimage.width() / double(width );

    double height_ratio = _qimage.height() / double(height);

    // ROI rect points
    int wid = rect_end.x() - rect_start.x();
    int hei = rect_end.y() - rect_start.y();

    double wid_ratio = double(wid) / _qimage.width();
    double height_mod = std::copysign(_qimage.height() * wid_ratio, hei);
    QPoint end = QPoint(rect_end.x(), rect_start.y() + height_mod);

    cv::Rect zoom_rect(cv::Point(rect_start.x(), rect_start.y()), cv::Point(end.x(), end.y()));
    double  scale_ratio = std::min(m_scroll_area_size.width() / double(zoom_rect.width), m_scroll_area_size.height() / double(zoom_rect.height));


    emit zoom_points(rect_start, end);
}




