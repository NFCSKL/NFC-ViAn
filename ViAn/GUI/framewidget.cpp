#include "framewidget.h"
#include <QDebug>
#include "Project/Analysis/analysis.h"
#include <QTime>
#include <QThread>
#include <QShortcut>
#include <QMessageBox>
#include "utility.h"

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
        copied_item->set_text_size(text->get_text_size());
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
    Shapes* new_item;
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
        new_item->set_text_size(text->get_text_size());
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

void FrameWidget::set_rotation(int rotation) {
    int width = m_org_image.cols;
    int height = m_org_image.rows;
    //qDebug() << "width height" << width << height;
    if (m_rotation == 90 || m_rotation == 270) {
        std::swap(width, height);
    }
    qDebug() << "rotation" << rotation;
    qDebug() << "m_rotation" << m_rotation;
    int diff_rotation = rotation - m_rotation;
    if (diff_rotation < 0) diff_rotation += 360;

    // Should not be used
    //rect_start = Utility::rotate(rect_start, diff_rotation, width, height);
    //rect_end = Utility::rotate(rect_end, diff_rotation, width, height);
    qDebug() << "rect_start" << rect_start;
    qDebug() << "rect_end" << rect_end;


    m_rotation = rotation;

    if (m_vid_proj) {
        m_vid_proj->get_video()->state.rotation = rotation;
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
        //setCursor(QCursor(QPixmap("../ViAn/Icons/pen.png")));  a way to use custom cursors
    case EDIT:
        setCursor(Qt::SizeAllCursor);
        break;
    default:
        unsetCursor();
        break;
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

cv::Mat FrameWidget::get_org_frame() const {
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
    m_org_image = org_image;
    switch (mod_image.type()) {
        case CV_8UC1:
            cvtColor(mod_image, _tmp_frame, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(mod_image, _tmp_frame, CV_BGR2RGB);
            break;
        case CV_8UC4:
            cvtColor(mod_image, _tmp_frame, CV_BGRA2RGB);
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
    painter.drawImage(QPoint(0,0), _qimage);
    // Draw the zoom box
    if (mark_rect) {
        painter.setPen(QColor(0,255,0));
        QRectF zoom(scale_to_view(rect_start), scale_to_view(rect_end));
        qDebug() << "in paint start" << rect_start << zoom.topLeft();
        painter.drawRect(zoom);
    }
    // Draw the select-analysis-area box
    if(m_tool == ANALYSIS_BOX){
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
        QRectF current_rect(scale_to_view(tl), scale_to_view(br));
        painter.setPen(Qt::black);
        painter.drawRect(current_rect);
        QPen pen(Qt::white, 1, Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(current_rect);
    }
    painter.end();
}

QPoint FrameWidget::scale_to_video(QPoint pos) {
    qDebug() << "view" << pos;
    QPoint scaled_pos = anchor + pos/m_scale_factor;
    qDebug() << "scaled pos" << scaled_pos << anchor;
    QPoint scaled_pos_no_anch = pos/m_scale_factor;

    // Rotated first then scale and lastly add anchor.
    QPoint rot_pos = Utility::rotate(pos, m_rotation, this->width(), this->height());
    qDebug() << "SCROLL AREA SIZE" << this->width() << this->height();
    QPoint r_pos = anchor + rot_pos/m_scale_factor;

    int width = m_org_image.cols;
    int height = m_org_image.rows;
    //qDebug() << "width height" << width << height;
//    if (m_rotation == 90 || m_rotation == 270) {
//        std::swap(width, height);
//    }
    QPoint q_pos = Utility::rotate(scaled_pos_no_anch, m_rotation, width, height);
    //qDebug() << "before anchor" << q_pos;
    //qDebug() << "anchor" << anchor;
    q_pos += anchor;
    qDebug() << r_pos;
    return r_pos;
}

QPoint FrameWidget::scale_to_view(QPoint pos) {
    //qDebug() << "pos at start" << pos;
    QPoint new_pos = pos-anchor;
    //qDebug() << "t,p frame" << _tmp_frame.cols << _tmp_frame.rows;

    QPoint q_pos = rotate(pos);
    //qDebug() << "-------" << q_pos;
    //QPoint rotated_anchor = Utility::rotate(anchor, m_rotation, m_org_image.cols, m_org_image.rows);
    QPoint scaled_pos = (pos-anchor)*m_scale_factor;
    QPoint r_pos = rotate(scaled_pos);
    //qDebug() << "anchor" << anchor;
    //qDebug() << "scaled" << scaled_pos;
    return r_pos;
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
    QPoint scaled_pos = scale_to_video(event->pos());
    emit mouse_double_click(scaled_pos);
}

/**
 * @brief FrameWidget::mousePressEvent
 * @param event
 */
void FrameWidget::mousePressEvent(QMouseEvent *event) {
    // Pos when the frame is at 100%
    QPoint scaled_pos = scale_to_video(event->pos());
    qDebug() << "event & scaled" << event->pos() << scaled_pos;
    switch (m_tool) {
    case ANALYSIS_BOX:
        if(event->button() == Qt::LeftButton) {
            ana_rect_start = event->pos();
            ana_rect_end = event->pos();
        }
        break;
    case ZOOM: {
        prev_point = event->pos();
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
                end_zoom();
                mark_rect = false;
                unsetCursor();
            } else {
                qDebug() << "rect start" << scaled_pos;
                rect_start = scaled_pos;
                qDebug() << "rect state at the start" << rect_start;
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
    QPoint scaled_pos = scale_to_video(event->pos());
    switch (m_tool) {
    case ANALYSIS_BOX:
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
    QPoint scaled_pos = scale_to_video(event->pos());
    switch (m_tool) {
    case ANALYSIS_BOX:
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
            //rect_end = rect_update(scale_point(event->pos()));
            qDebug() << "rect end" << scaled_pos;
            rect_end = scaled_pos;
            //rect_end = rotate(scaled_pos);
            repaint();
        } else {
            if (zoom_rect.contains(scaled_pos) && mark_rect) {
                setCursor(QCursor(QPixmap("../ViAn/Icons/zoom_in.png")));
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
                panning(event->pos()+QPoint(-PAN_FACTOR*m_scale_factor,0));
            } else {
                panning(event->pos()+QPoint(PAN_FACTOR*m_scale_factor,0));
            }
        }
        else if (event->modifiers() == Qt::ControlModifier) {
            if (num_steps.y() < 0) {
                emit move_viewport_center(scaled_pos, 1/ZOOM_STEP);
            } else {
                emit move_viewport_center(scaled_pos, ZOOM_STEP);
            }
        } else {
            init_panning(event->pos());
            if (num_steps.y() < 0) {
                panning(event->pos()+QPoint(0,-PAN_FACTOR*m_scale_factor));
            } else {
                panning(event->pos()+QPoint(0,PAN_FACTOR*m_scale_factor));
            }
        }
    default:
        break;
    }

}

void FrameWidget::set_analysis_settings() {
    QMessageBox msg_box;
    msg_box.setText("Quick analysis");
    msg_box.setInformativeText("Do you wanna start an analysis on the marked area?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = msg_box.exec();

    if (reply == QMessageBox::Yes) {
        AnalysisSettings* settings = new AnalysisSettings(MOTION_DETECTION);
        settings->quick_analysis = true;

        QPoint scaled_start = scale_to_video(ana_rect_start);
        QPoint scaled_end = scale_to_video(ana_rect_end);
        QRect scaled_rect(scaled_start, scaled_end);
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

    qDebug() << panning_tracker.first << panning_tracker.second;

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

QPoint FrameWidget::rotate(QPoint pos) {
    int width = this->width();
    int height = this->height();
    if (m_rotation == 90 || m_rotation == 270) {
        std::swap(width, height);
    }
    QPoint point = Utility::rotate(pos, 360-m_rotation, width, height);
    return point;
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
    qDebug() << rect_start << rect_end;
    //emit zoom_points(rotate(rect_start), rotate(rect_end));
    emit zoom_points(rect_start, rect_end);
}
