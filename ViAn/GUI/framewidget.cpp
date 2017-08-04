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
    ooi_rects.clear();
}

void FrameWidget::set_video_project(VideoProject* vid_proj) {
    m_vid_proj = vid_proj;
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

void FrameWidget::set_overlay(Overlay* overlay) {
    video_overlay = overlay;
}

Overlay* FrameWidget::get_overlay() {
    return video_overlay;
}

void FrameWidget::set_anchor(QPoint p) {
    anchor = p;
}

void FrameWidget::update(){
    repaint();
}

void FrameWidget::set_tool(SHAPES tool) {
    if (m_vid_proj != nullptr) {

        if (tool == TEXT) {
            QString current_string = "Enter text";
            float current_font_scale = 1;
            std::string input_string = current_string.toStdString();
            float input_font_scale = current_font_scale;
            CustomDialog dialog("Choose text", NULL);
            dialog.addLabel("Enter values:");
            dialog.addLineEdit ("Text:", &input_string, "Enter a text that can then be used to draw on the overlay.");
            dialog.addDblSpinBoxF("Font scale:", Text::FONT_SCALE_MIN, Text::FONT_SCALE_MAX,
                                  &input_font_scale, Text::FONT_SCALE_DECIMALS, Text::FONT_SCALE_STEP,
                                  "Choose font scale, 0.5 to 5.0 (this value is multiplied with a default font size).");

            // Show the dialog (execution will stop here until the dialog is finished)
            dialog.exec();

            if (!dialog.wasCancelled() && !input_string.empty()) {
                // Not cancelled and not empty field.
                current_string = QString::fromStdString(input_string);
                current_font_scale = input_font_scale;
            }
            emit send_tool_text(current_string, current_font_scale);
        } else {
            emit send_tool(tool);
        }

        this->tool = tool;
    }
}

void FrameWidget::set_overlay_color(QColor color) {
    if (m_vid_proj != nullptr) {
        emit send_color(color);
        this->overlay_color = color;
    }
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

QPoint FrameWidget::scale_point(QPoint pos) {
    return anchor + pos/m_scale_factor;
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
    case NONE:
        break;
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
        emit mouse_pressed(scale_point(event->pos()));
        break;
    }
}

/**
 * @brief FrameWidget::mouseReleaseEvent
 * @param event
 */
void FrameWidget::mouseReleaseEvent(QMouseEvent *event) {
    switch (tool) {
    case NONE:
        break;
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
        int wid = rect_end.x() - rect_start.x();
        int hei = rect_end.y() - rect_start.y();

        double wid_ratio = double(wid) / _qimage.width();
        double height_mod = std::copysign(_qimage.height() * wid_ratio, hei);
        cv::Point end = cv::Point(rect_end.x(), rect_start.y() + height_mod);
        cv::Point start (rect_start.x(), rect_start.y());
        cv::Rect scaled = cv::Rect(cv::Point(anchor.x() + start.x / m_scale_factor, anchor.y() + start.y / m_scale_factor),
                      cv::Point(anchor.x() + end.x / m_scale_factor, anchor.y() + end.y / m_scale_factor));

        settings->setBounding_box(scaled);
        emit quick_analysis(settings);
        qDebug () << "TODO:analysis rect needs scaling";
        tool = NONE;
        mark_rect = false;
        break;
    }
    default:
        emit mouse_released(scale_point(event->pos()));
        break;
    }
}

/**
 * @brief FrameWidget::mouseMoveEvent
 * @param event
 */
void FrameWidget::mouseMoveEvent(QMouseEvent *event) {
    switch (tool) {
    case NONE:
        break;
    case ANALYSIS_BOX:

    case ZOOM:
        if (event->buttons() == Qt::RightButton){
            panning(event->pos());
        } else if (event->buttons() == Qt::LeftButton && !do_zoom_out) {
            rect_update(event->pos());
        }
        break;
    default:
        emit mouse_moved(scale_point(event->pos()));
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

    // ROI rect points
    int wid = rect_end.x() - rect_start.x();
    int hei = rect_end.y() - rect_start.y();

    double wid_ratio = double(wid) / _qimage.width();
    double height_mod = std::copysign(_qimage.height() * wid_ratio, hei);
    QPoint end = QPoint(rect_end.x(), rect_start.y() + height_mod);





    emit zoom_points(rect_start, end);
}
