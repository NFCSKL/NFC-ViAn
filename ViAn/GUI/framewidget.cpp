#include "framewidget.h"
#include <QDebug>
#include "Project/Analysis/analysis.h"
#include <QTime>
#include <QThread>

FrameWidget::FrameWidget(QWidget *parent) : QWidget(parent) {}

void FrameWidget::toggle_zoom(bool value) {
    if (value) {
        tool = ZOOM;
        emit send_tool(ZOOM);
        setCursor(Qt::CrossCursor);
    } else {
        unsetCursor();
        tool = NONE;
        emit send_tool(NONE);
    }
}

void FrameWidget::set_analysis_tool(bool status) {
    if (status) {
        tool = ANALYSIS_BOX;
        setCursor(Qt::CrossCursor);
    } else {
        tool = NONE;
        unsetCursor();
    }
}


void FrameWidget::set_scroll_area_size(QSize size) {
    m_scroll_area_size = size;
}

/**
 * @brief FrameWidget::set_analysis
 * @param analysis
 * Set the analysis and the pois on the current frame
 */
void FrameWidget::set_analysis(AnalysisProxy *analysis) {
    m_analysis = analysis->load_analysis();
    set_detections_on_frame(current_frame_nr);
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

void FrameWidget::show_bounding_box(bool b) {
    show_box = b;
    repaint();
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

void FrameWidget::set_current_frame_nr(int nr) {
    current_frame_nr = nr;
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

cv::Mat FrameWidget::get_modified_frame() const {
    return _tmp_frame.clone();
}

cv::Mat FrameWidget::get_org_frame() const
{
    cv::Mat tmp = m_org_image.clone();
    switch (tmp.type()) {
        case CV_8UC1:
            cvtColor(m_org_image, tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(m_org_image, tmp, CV_BGR2RGB);
            break;
    }
    return tmp;
}

void FrameWidget::on_new_image(cv::Mat org_image, cv::Mat mod_image, int frame_index) {
    current_frame = mod_image;
    m_org_image = org_image;
    switch (mod_image.type()) {
        case CV_8UC1:
            cvtColor(mod_image, _tmp_frame, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(mod_image, _tmp_frame, CV_BGR2RGB);
            break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(mod_image.isContinuous());
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
        // Draw the zoom box with correct dimension
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
        // Draw the zoom box
        painter.setPen(QColor(0,255,0));
        QRectF zoom(rect_start, rect_end);
        painter.drawRect(zoom);
    }


    if(tool == ANALYSIS_BOX){
        painter.setPen(QColor(0,255,0));
        QRectF analysis(ana_rect_start, ana_rect_end);
        painter.drawRect(analysis);
    }
    if (show_box && m_analysis != nullptr) {
        painter.setPen(QColor(180,200,200));
        QPoint tl(m_analysis->bounding_box.x, m_analysis->bounding_box.y);
        QPoint br(m_analysis->bounding_box.x+m_analysis->bounding_box.width, m_analysis->bounding_box.y+m_analysis->bounding_box.height);
        QRectF bounding_rect((tl-anchor)*m_scale_factor, (br-anchor)*m_scale_factor);
        painter.drawRect(bounding_rect);
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
        if(event->button() == Qt::LeftButton) {
            ana_rect_start = event->pos();
            ana_rect_end = event->pos();
        }
        break;
    case ZOOM:
        if (event->button() == Qt::RightButton) {
            init_panning(event->pos());
        } else if (event->button() == Qt::LeftButton) {
            if (event->modifiers() == Qt::ControlModifier) {
                do_zoom_out = true;
            } else {
                rect_start = event->pos();
            }
        }
        break;
    default:
        bool right_click = (event->button() == Qt::RightButton);
        emit mouse_pressed(scale_point(event->pos()), right_click);
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
        set_analysis_settings();
        break;
    }
    default:
        emit mouse_released(scale_point(event->pos()), false);
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
        if (event->buttons() == Qt::LeftButton) {
            ana_rect_end = rect_update(event->pos());
        }
        break;
    case ZOOM:
        if (event->buttons() == Qt::RightButton){
            panning(event->pos());
        } else if (event->buttons() == Qt::LeftButton && !do_zoom_out) {
            rect_end = rect_update(event->pos());
            mark_rect = true;
        }
        break;
    default:
        emit mouse_moved(scale_point(event->pos()));
        break;
    }
}

/**
 * @brief FrameWidget::wheelEvent
 * @param event
 */
void FrameWidget::wheelEvent(QWheelEvent *event) {
    QPoint num_degree = event->angleDelta() / 8;
    QPoint num_steps = num_degree / 15;
    emit mouse_scroll(num_steps);
    event->accept();
}

void FrameWidget::set_analysis_settings() {
    QMessageBox msg_box;
    msg_box.setText("Quick analysis");
    msg_box.setInformativeText("Do you wanna start an analysis on the marked area?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = msg_box.exec();

    if (reply == QMessageBox::Yes) {
        AnalysisSettings* settings = new AnalysisSettings(MOTION_DETECTION);

        cv::Point end = cv::Point(ana_rect_end.x(), ana_rect_end.y());
        cv::Point start (ana_rect_start.x(), ana_rect_start.y());
        cv::Rect scaled = cv::Rect(cv::Point(anchor.x()/m_scale_factor + start.x / m_scale_factor, anchor.y()/m_scale_factor + start.y / m_scale_factor),
                      cv::Point(anchor.x()/m_scale_factor + end.x / m_scale_factor, anchor.y()/m_scale_factor + end.y / m_scale_factor));
        settings->setBounding_box(scaled);

        emit quick_analysis(settings);
    }
    ana_rect_end = ana_rect_start;
    repaint();
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
QPoint FrameWidget::rect_update(QPoint pos) {
    // Force image boundries
    int tmpx = std::min(pos.x(),_qimage.width()-1);
    int ex = std::max(0, tmpx);
    int tmpy = std::min(pos.y(), _qimage.height()-1);
    int ey = std::max(0,tmpy);
    repaint();
    return QPoint(ex, ey);
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
