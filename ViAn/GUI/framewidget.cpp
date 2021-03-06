#include "framewidget.h"

#include "Analysis/analysissettings.h"
#include "constants.h"
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/videoproject.h"
#include "utility.h"
#include "Video/shapes/arrow.h"
#include "Video/shapes/circle.h"
#include "Video/shapes/line.h"
#include "Video/shapes/pen.h"
#include "Video/shapes/rectangle.h"
#include "Video/shapes/text.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QClipboard>
#include <QDebug>
#include <QGuiApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>
#include <QWheelEvent>


FrameWidget::FrameWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    QShortcut* copy_sc = new QShortcut(this);
    QShortcut* paste_sc = new QShortcut(this);
    copy_sc->setKey(QKeySequence::Copy);
    paste_sc->setKey(QKeySequence::Paste);
    connect(copy_sc, &QShortcut::activated, this, &FrameWidget::copy);
    connect(paste_sc, &QShortcut::activated, this, &FrameWidget::paste);
}

/**
 * @brief FrameWidget::copy
 * Saves a copy of the current item which can be used to later create new copies.
 */
void FrameWidget::copy() {
    if (copied_item) delete copied_item;
    Shapes* current_drawing = m_vid_proj->get_overlay()->get_current_drawing();
    if (!current_drawing) return;
    switch (current_drawing->get_shape()) {
    case RECTANGLE:
        copied_item = new Rectangle(current_drawing->get_color(), QPoint(0,0));
        break;
    case CIRCLE:
        copied_item = new Circle(current_drawing->get_color(), QPoint(0,0));
        break;
    case LINE:
        copied_item = new Line(current_drawing->get_color(), QPoint(0,0));
        break;
    case ARROW:
        copied_item = new Arrow(current_drawing->get_color(), QPoint(0,0));
        break;
    case PEN: {
        Pen* pen = dynamic_cast<Pen*>(current_drawing);
        copied_item = new Pen(pen->get_color(), QPoint(0,0));
        Pen* copied_item_pen = dynamic_cast<Pen*>(copied_item);
        copied_item_pen->set_points(pen->get_points());
        break;
    }
    case TEXT: {
        Text* text = dynamic_cast<Text*>(current_drawing);
        copied_item = new Text(text->get_color(), QPoint(0,0), text->get_name(), text->get_font_scale());
        Text* copied_item_text = dynamic_cast<Text*>(copied_item);
        copied_item_text->set_text_size(text->get_text_size());
        break;
    }
    default:
        break;
    }
    copied_item->set_name(current_drawing->get_name());
    copied_item->set_thickness(current_drawing->get_thickness());
    copied_item->set_draw_start(current_drawing->get_draw_start());
    copied_item->set_draw_end(current_drawing->get_draw_end());
}

/**
 * @brief FrameWidget::paste
 * Creates a new item which is a copy of the item in the copy clipboard
 * and adds it to the overlay.
 */
void FrameWidget::paste() {
    if (!copied_item) return;
    Shapes* new_item = nullptr;
    switch (copied_item->get_shape()) {
    case RECTANGLE:
        new_item = new Rectangle(copied_item->get_color(), QPoint(0,0));
        break;
    case CIRCLE:
        new_item = new Circle(copied_item->get_color(), QPoint(0,0));
        break;
    case LINE:
        new_item = new Line(copied_item->get_color(), QPoint(0,0));
        break;
    case ARROW:
        new_item = new Arrow(copied_item->get_color(), QPoint(0,0));
        break;
    case PEN: {
        Pen* pen = dynamic_cast<Pen*>(copied_item);
        new_item = new Pen(pen->get_color(), QPoint(0,0));
        Pen* new_pen = dynamic_cast<Pen*>(new_item);
        new_pen->set_points(pen->get_points());
        break;
    }
    case TEXT: {
        Text* text = dynamic_cast<Text*>(copied_item);
        new_item = new Text(text->get_color(), QPoint(0,0), text->get_name(), text->get_font_scale());
        Text* new_text = dynamic_cast<Text*>(new_item);
        new_text->set_text_size(text->get_text_size());
        break;
    }
    default:
        break;
    }

    new_item->set_name(copied_item->get_name());
    new_item->set_thickness(copied_item->get_thickness());
    new_item->set_draw_start(copied_item->get_draw_start());
    new_item->set_draw_end(copied_item->get_draw_end());
    m_vid_proj->get_overlay()->add_drawing(new_item, current_frame_nr);
    emit process_frame();
    update();
}

// TODO Not need?
void FrameWidget::set_scroll_area_size(QSize size) {
    m_scroll_area_size = size; //Unused
}

/**
 * @brief FrameWidget::set_analysis
 * @param analysis
 * Set the analysis and the pois on the current frame
 */
void FrameWidget::set_analysis(AnalysisProxy *analysis) {
    if (current_analysis != analysis) {
        clear_analysis();
        m_analysis = analysis->load_analysis();
        set_detections_on_frame(current_frame_nr);
        current_analysis = analysis;
    }
}

/**
 * @brief FrameWidget::clear_analysis
 * Forgets the current analysis
 */
void FrameWidget::clear_analysis() {
    delete m_analysis;
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

/**
 * @brief FrameWidget::set_details_checked
 * Details_checked will be true when the details setting is checked
 * @param b
 */
void FrameWidget::set_details_checked(bool b) {
    details_checked = b;
    repaint();
}

/**
 * @brief FrameWidget::set_show_box
 * Show_box will be true when the details box should be shown.
 * Only when an analysis is clicked
 * @param b
 */
void FrameWidget::set_show_box(bool b) {
    show_box = b;
}

void FrameWidget::set_detections(bool detections) {
    m_detections = detections;
}

void FrameWidget::set_show_detections(bool show) {
    show_detections = show;
}

void FrameWidget::set_anchor(QPoint p) {
    anchor = p;
    if (m_vid_proj) {
        m_vid_proj->get_video()->state.anchor = p;
    }
}

void FrameWidget::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;

    if (m_vid_proj) {
        m_vid_proj->get_video()->state.scale_factor = scale_factor;
    }
}

/**
 * @brief FrameWidget::set_rotation
 * Slot function for updating framewidget after rotation have been done
 * Updates the zoom rectangle coordinates as well as the rotation held by the current video project state
 * @param rotation
 */
void FrameWidget::set_rotation(int rotation) {
    int diff_rotation = rotation - m_rotation;
    if (diff_rotation < Constants::DEGREE_MIN) diff_rotation += Constants::DEGREE_MAX;
    m_rotation = rotation;

    if (m_vid_proj) {
        m_vid_proj->get_video()->state.rotation = rotation;
    }
}

void FrameWidget::set_flip(bool flip_h, bool flip_v) {
    // flip_h & flip_v ---- current flip state
    m_flip_h = flip_h;
    m_flip_v = flip_v;
    if (m_vid_proj) {
        m_vid_proj->get_video()->state.flip_h = flip_h;
        m_vid_proj->get_video()->state.flip_v = flip_v;
    }
}

void FrameWidget::set_current_frame_nr(int nr) {
    current_frame_nr = nr;
}

void FrameWidget::update(){
    repaint();
}

void FrameWidget::set_tool(SHAPES tool) {
    mark_rect = false;
    rect_start = QPoint(0,0);
    rect_end = QPoint(0,0);
    emit send_tool(tool);
    m_tool = tool;
    set_cursor(m_tool);
    repaint();
}

/**
 * @brief FrameWidget::set_cursor
 * @param tool
 * Set the cursor depending on the current tool
 */
void FrameWidget::set_cursor(SHAPES tool) {
    switch (tool) {
    case ZOOM:
        unsetCursor();
        break;
    case ANALYSIS_BOX:
    case OBJECT_DETECT_BOX:
    case RECTANGLE:
    case CIRCLE:
    case PEN:
    case ARROW:
    case LINE:
    case TEXT:
        setCursor(Qt::CrossCursor);
        break;
    case MOVE:
        break;
        //setCursor(QCursor(QPixmap(":/Icons/pen.png")));  a way to use custom cursors
    case EDIT:
        setCursor(Qt::SizeAllCursor);
        break;
    }
}

void FrameWidget::set_overlay_color(QColor color) {
    if (m_vid_proj != nullptr) {
        emit send_color(color);
        this->overlay_color = color;
    }
}

/**
 * @brief FrameWidget::copy_to_clipboard
 * Copy the current frame to the clipboard for use in other applications
 */
void FrameWidget::copy_to_clipboard() {
    cv::Mat img2 = _tmp_frame.clone();

    QImage _qimage = QImage(img2.data, img2.cols, img2.rows, img2.cols*3, QImage::Format_RGB888);

    QPixmap pix;
    pix = QPixmap::fromImage(_qimage);

    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(pix);
}

cv::Mat FrameWidget::get_modified_frame() const {
    return _tmp_frame.clone();
}

cv::Mat FrameWidget::get_org_frame() const {
    cv::Mat tmp = m_org_image.clone();
    switch (tmp.type()) {
        case CV_8UC1:
            cvtColor(m_org_image, tmp, cv::COLOR_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(m_org_image, tmp, cv::COLOR_BGR2RGB);
            break;
    }
    return tmp;
}

void FrameWidget::on_new_image(cv::Mat org_image, cv::Mat mod_image, int frame_index) {
    if (frame_is_clear) return;
    current_frame_nr = frame_index;
    m_org_image = org_image;
    switch (mod_image.type()) {
        case CV_8UC1:
            cvtColor(mod_image, _tmp_frame, cv::COLOR_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(mod_image, _tmp_frame, cv::COLOR_BGR2RGB);
            break;
        case CV_8UC4:
            cvtColor(mod_image, _tmp_frame, cv::COLOR_BGRA2RGB);
            break;
        default:
            qWarning() << "Mat was of unknown type [" << mod_image.type() << "]";
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
    if (!isVisible()) show();
    repaint();
}

/**
 * @brief FrameWidget::paintEvent
 * @param event
 */
void FrameWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED (event)
    QPainter painter(this);
    if (frame_is_clear) {
        _qimage = QImage(m_scroll_area_size, QImage::Format_RGB32);
        setFixedSize(_qimage.size());
    }
    painter.drawImage(QPoint(0,0), _qimage);
    // Draw the zoom box
    if (mark_rect) {
        painter.setPen(QColor(0,255,0));
        QRectF zoom(scale_to_view(rect_start), scale_to_view(rect_end));
        painter.drawRect(zoom);
    }
    // Draw the select-analysis-area box
    if(m_tool == ANALYSIS_BOX || m_tool == OBJECT_DETECT_BOX) {
        painter.setPen(QColor(0,255,0));
        QRectF analysis(ana_rect_start, ana_rect_end);
        painter.drawRect(analysis);
    }
    // Draw analysis details bounding box
    if (details_checked && show_box && m_analysis != nullptr) {
        painter.setPen(QColor(180,200,200));

        auto box = m_analysis->settings->bounding_box;
        QPoint tl = Utility::from_cvpoint(box.tl());
        QPoint br = Utility::from_cvpoint(box.br());
        QRectF bounding_rect(scale_to_view(tl), scale_to_view(br));
        painter.drawRect(bounding_rect);
    }
    // Draw analysis detection boxes
    if (m_detections && show_detections) {
        for (cv::Rect rect : ooi_rects) {
            QPoint tl(rect.x, rect.y);
            QPoint br(rect.x+rect.width, rect.y+rect.height);
            QRectF detect_rect(scale_to_view(tl), scale_to_view(br));
            painter.setPen(QColor(0,0,255));
            painter.drawRect(detect_rect);
        }
    }
    // TODO This is to prevent crash when clicking bookmark before loading video
    // This should get a better fix later
    if (!m_vid_proj) {
        painter.end();
        return;
    }
    // Draw the current drawing bounding box
    Shapes* current_drawing = m_vid_proj->get_overlay()->get_current_drawing();
    bool show_overlay = m_vid_proj->get_overlay()->get_show_overlay();
    if (show_overlay && current_drawing && current_frame_nr == current_drawing->get_frame() && m_tool == EDIT) {
        QPoint tl;
        QPoint br;
        if (current_drawing->get_shape() == PEN) {
            Pen* current = dynamic_cast<Pen*>(current_drawing);
            cv::Rect bounding_rect = cv::boundingRect(current->get_points());
            tl = Utility::from_cvpoint(bounding_rect.tl());
            br = Utility::from_cvpoint(bounding_rect.br());
        } else {
            tl = Utility::from_cvpoint(current_drawing->get_draw_start());
            br = Utility::from_cvpoint(current_drawing->get_draw_end());
        }
        QRectF current_rect;
        if (current_drawing->get_shape() == TEXT) {
            Text* current = dynamic_cast<Text*>(current_drawing);
            br = Utility::from_cvpoint(current->get_text_draw_end());
            current_rect = QRectF(scale_to_view(tl), scale_to_view(br));
        } else {
            current_rect = QRectF(scale_to_view(tl), scale_to_view(br));
        }
        painter.setPen(Qt::black);
        painter.drawRect(current_rect);
        QPen pen(Qt::white, 1, Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(current_rect);
    }
    painter.end();
}

/**
 * @brief FrameWidget::scale_to_video
 * Scale a point to the coordinates in the video
 * @param pos   : point in the viewport
 * @return
 */
QPoint FrameWidget::scale_to_video(QPoint pos) {
    QPoint scaled_pos = pos;
    scaled_pos = anchor + scaled_pos/m_scale_factor;
    scaled_pos = rotate(scaled_pos, Constants::DEGREE_MAX-m_rotation);
    return scaled_pos;
}

/**
 * @brief FrameWidget::scale_to_view
 * Scale and rotate a point to coordinates in the viewport
 * @param pos       : point in the video
 * @param rotate    : if the point should be rotated
 * @return
 */
QPoint FrameWidget::scale_to_view(QPoint pos) {
    int width = m_org_image.cols;
    int height = m_org_image.rows;

    pos = Utility::flip(pos, m_flip_h, m_flip_v, width, height);
    pos = Utility::rotate(pos, m_rotation, width, height);
    QPoint scaled_pos = (pos-anchor)*m_scale_factor;
    return scaled_pos;
}

/**
 * @brief FrameWidget::resizeEvent
 * @param event
 */
void FrameWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED (event)
    // TODO unused
    emit current_size(width(), height());
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (frame_is_clear) return;
    QPoint scaled_pos = scale_to_video(event->pos());
    emit mouse_double_click(scaled_pos);
}

/**
 * @brief FrameWidget::mousePressEvent
 * @param event
 */
void FrameWidget::mousePressEvent(QMouseEvent *event) {
    if (frame_is_clear) return;
    // Pos when the frame is at 100%
    QPoint scaled_pos = scale_to_video(event->pos());
    switch (m_tool) {
    case ANALYSIS_BOX:
    case OBJECT_DETECT_BOX:
        if(event->button() == Qt::LeftButton) {
            ana_rect_start = event->pos();
            ana_rect_end = event->pos();
        }
        break;
    case ZOOM: {
        prev_point_scaled = scaled_pos;
        QRect zoom_rect(rect_start, rect_end);
        if (event->button() == Qt::RightButton) {
            if (zoom_rect.contains(scaled_pos)) {
                pan_rect = true;
            } else {
                pan_rect = false;
                init_panning(event->pos());
            }
        } else if (event->button() == Qt::LeftButton) {
            if (zoom_rect.contains(scaled_pos)) {
                if (rect_start == rect_end) return;
                end_zoom();
                mark_rect = false;
                unsetCursor();
            } else {
                rect_start = scaled_pos;
                mark_rect = true;
            }
            rect_end = rect_start;
            repaint();
        }
        break;
    }
    case MOVE:
        init_panning(event->pos());
        break;
    default:
        // Rotate here
        bool right_click = (event->button() == Qt::RightButton);
        emit mouse_pressed(scaled_pos, right_click);
        break;
    }
}

/**
 * @brief FrameWidget::mouseReleaseEvent
 * @param event
 */
void FrameWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (frame_is_clear) return;
    QPoint scaled_pos = scale_to_video(event->pos());
    switch (m_tool) {
    case ANALYSIS_BOX:
    case OBJECT_DETECT_BOX:
        set_analysis_settings();
        break;
    case ZOOM:
        if (event->button() == Qt::RightButton) {
            end_panning();
        } else if (event->button() == Qt::LeftButton){}
        break;
    case MOVE:
        end_panning();
        break;
    default:
        emit mouse_released(scaled_pos, false);
        break;
    }
}

/**
 * @brief FrameWidget::mouseMoveEvent
 * @param event
 */
void FrameWidget::mouseMoveEvent(QMouseEvent *event) {
    if (frame_is_clear) return;
    QPoint scaled_pos = scale_to_video(event->pos());
    switch (m_tool) {
    case ANALYSIS_BOX:
    case OBJECT_DETECT_BOX:
        if (event->buttons() == Qt::LeftButton) {
            ana_rect_end = rect_update(event->pos());
        }
        break;
    case ZOOM: {
        QRect zoom_rect(rect_start, rect_end);
        if (event->buttons() == Qt::RightButton){
            if (pan_rect && mark_rect) {
                QPoint diff_point = scaled_pos - prev_point_scaled;

                rect_start += diff_point;
                rect_end += diff_point;
                prev_point_scaled = scaled_pos;
                repaint();
            } else {
                panning(event->pos());
            }
        } else if (event->buttons() == Qt::LeftButton && mark_rect) {
            rect_end = scaled_pos;
            repaint();
        } else {
            if (zoom_rect.contains(scaled_pos) && mark_rect) {
                setCursor(QCursor(QPixmap(":/Icons/new_zoom.png")));
            } else {
                unsetCursor();
            }
        }
        break;
    }
    case MOVE:
        if (event->buttons() == Qt::LeftButton) {
            panning(event->pos());
        }
        break;
    default:
        if (event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton) {
            bool shift = (event->modifiers() == Qt::ShiftModifier);
            bool ctrl = (event->modifiers() == Qt::ControlModifier);
            emit mouse_moved(scaled_pos, shift, ctrl);
        }
        break;
    }
}

/**
 * @brief FrameWidget::wheelEvent
 * @param event
 */
void FrameWidget::wheelEvent(QWheelEvent *event) {
    if (frame_is_clear) return;
    QPoint scaled_pos = scale_to_video(event->pos());
    QPoint num_degree = event->angleDelta() / 8;
    QPoint num_steps = num_degree / 15;
    switch (m_tool) {
    case EDIT:
        emit mouse_scroll(num_steps);
        event->accept();
        break;
    case MOVE:
        break;
    case ZOOM:
        if (event->modifiers() == Qt::ShiftModifier) {
            init_panning(event->pos());
            if (num_steps.y() < 0) {
                panning(event->pos()+QPoint(-Constants::PAN_FACTOR*m_scale_factor,0));
            } else {
                panning(event->pos()+QPoint(Constants::PAN_FACTOR*m_scale_factor,0));
            }
        }
        else if (event->modifiers() == Qt::ControlModifier) {
            if (num_steps.y() < 0) {
                emit move_viewport_center(scaled_pos, 1/Constants::ZOOM_STEP);
            } else {
                emit move_viewport_center(scaled_pos, Constants::ZOOM_STEP);
            }
        } else {
            init_panning(event->pos());
            if (num_steps.y() < 0) {
                panning(event->pos()+QPoint(0,-Constants::PAN_FACTOR*m_scale_factor));
            } else {
                panning(event->pos()+QPoint(0,Constants::PAN_FACTOR*m_scale_factor));
            }
        }
        break;
    default:
        break;
    }

}

void FrameWidget::set_analysis_settings() {
    QMessageBox msg_box;
    if (m_tool == ANALYSIS_BOX) {
        msg_box.setText("Motion detection");
    } else if (m_tool == OBJECT_DETECT_BOX) {
        msg_box.setText("Object detection");
    } else {
        msg_box.setText("Quick analysis");
    }
    msg_box.setMinimumSize(240,130);
    msg_box.setInformativeText("Do you wanna start detecting on the marked area?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = msg_box.exec();

    if (reply == QMessageBox::Yes) {
        AnalysisSettings* settings;
        if (m_tool == ANALYSIS_BOX) {
            settings = new AnalysisSettings(MOTION_DETECTION);
        } else if (m_tool == OBJECT_DETECT_BOX) {
            settings = new AnalysisSettings(OBJECT_DETECTION);
        } else {
            settings = new AnalysisSettings;
        }
        settings->quick_analysis = true;

        QRect scaled_rect(scale_to_video(ana_rect_start), scale_to_video(ana_rect_end));
        settings->set_bounding_box(Utility::from_qrect(scaled_rect));

        emit quick_analysis(settings);
        emit set_toolbar_zoom();
    }
    ana_rect_end = ana_rect_start;
    repaint();
}

/**
 * @brief FrameWidget::init_panning
 * Init panning on frame
 * @param pos
 */
void FrameWidget::init_panning(QPoint pos) {
    prev_pos = pos;
}

/**
 * @brief FrameWidget::panning
 * Is panning. Emits the new pos on frame.
 * @param pos
 */
void FrameWidget::panning(QPoint pos) {
    // Using panning
    int dx {prev_pos.x() - pos.x()};
    int dy {prev_pos.y() - pos.y()};

    panning_tracker.first += dx / m_scale_factor;
    panning_tracker.second += dy/ m_scale_factor;

    auto scale_panning = [] (double& actual, int& scaled) {
        if (std::abs(actual) >= 1) {
            scaled = std::floor(actual);
            actual -= scaled;
        }
    };

    int x{}, y{};
    scale_panning(panning_tracker.first, x);
    scale_panning(panning_tracker.second, y);
    emit moved_xy(x, y);
    prev_pos = pos;
}

/**
 * @brief FrameWidget::rotate
 * Will rotate the given point with the given rotation
 * @param pos           : Point to be rotated
 * @param rotation      : Rotation
 * @param swap          : If the width and height might need to be swaped
 * @return
 */
QPoint FrameWidget::rotate(QPoint pos, int rotation) {
    int width = m_org_image.cols;
    int height = m_org_image.rows;

    QPoint new_pos = pos;
    if (rotation == Constants::DEGREE_90 || rotation == Constants::DEGREE_270) {
        new_pos = Utility::rotate(new_pos, rotation, height, width);
    } else {
        new_pos = Utility::rotate(new_pos, rotation, width, height);
    }
    new_pos = Utility::flip(new_pos, m_flip_h, m_flip_v, width, height);
    return new_pos;
}

/**
 * @brief FrameWidget::rect_update
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
    if (m_tool == ZOOM) {
        unsetCursor();
    } else if (m_tool == MOVE){
        setCursor(Qt::OpenHandCursor);
    }
}

/**
 * @brief FrameWidget::end_zoom
 * Emits the points of the zooming rect
 */
void FrameWidget::end_zoom() {
    repaint();

    int width = m_org_image.cols;
    int height = m_org_image.rows;

    QPoint new_start = rect_start;
    QPoint new_end = rect_end;
    new_start = Utility::flip(new_start, m_flip_h, m_flip_v, width, height);
    new_start = Utility::rotate(new_start, m_rotation, width, height);
    new_end = Utility::flip(new_end, m_flip_h, m_flip_v, width, height);
    new_end = Utility::rotate(new_end, m_rotation, width, height);

    emit zoom_points(new_start, new_end);
}

void FrameWidget::clear_frame(bool value) {
    frame_is_clear = value;
}
