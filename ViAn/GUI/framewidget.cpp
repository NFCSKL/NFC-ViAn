#include "framewidget.h"
#include <QDebug>
#include "Project/Analysis/analysis.h"
#include <QTime>
#include <QThread>
#include <QShortcut>
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

void FrameWidget::set_scroll_area_size(QSize size) {
    m_scroll_area_size = size;
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
    mark_rect = false;
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
    case NONE:
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
        setCursor(Qt::OpenHandCursor);
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
    Q_UNUSED (event)
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);

    if (mark_rect) {
        // Draw the zoom box with correct dimension
        if(m_tool == ZOOM){
            QPoint start = rect_start;
            QPoint end = rect_end;

            int width = end.x() - start.x();
            int height = end.y() - start.y();

            double width_ratio = double(width) / _qimage.width();
            double height_mod = std::copysign(_qimage.height() * width_ratio, height);

            end = QPoint(end.x(), start.y() + height_mod);

            painter.setPen(QColor(255,0,0));
            QRectF correct_dim_rect((start-anchor)*m_scale_factor, (end-anchor)*m_scale_factor);
            painter.drawRect(correct_dim_rect);
        }
        // Draw the zoom box
        painter.setPen(QColor(0,255,0));
        QRectF zoom((rect_start-anchor)*m_scale_factor, (rect_end-anchor)*m_scale_factor);
        painter.drawRect(zoom);
    }

    if(m_tool == ANALYSIS_BOX){
        painter.setPen(QColor(0,255,0));
        QRectF analysis(ana_rect_start, ana_rect_end);
        painter.drawRect(analysis);
    }
    if (show_box && m_analysis != nullptr) {
        painter.setPen(QColor(180,200,200));

        auto box = m_analysis->settings->bounding_box;
        QPoint tl = Utility::from_cvpoint(box.tl());
        QPoint br = Utility::from_cvpoint(box.br());
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
    Shapes* current_drawing = m_vid_proj->get_overlay()->get_current_drawing();
    bool show_overlay = m_vid_proj->get_overlay()->get_show_overlay();
    if (show_overlay && current_drawing && current_frame_nr == current_drawing->get_frame() && m_tool == EDIT) {
        QPoint tl;
        QPoint br;
        if (current_drawing->get_shape() == PEN) {
            Pen* current = dynamic_cast<Pen*>(current_drawing);
            cv::Rect bounding_rect = cv::boundingRect(current->get_points());
            tl = QPoint(bounding_rect.tl().x, bounding_rect.tl().y);
            br = QPoint(bounding_rect.br().x, bounding_rect.br().y);
        } else {
            tl = QPoint(current_drawing->get_draw_start().x, current_drawing->get_draw_start().y);
            br = QPoint(current_drawing->get_draw_end().x, current_drawing->get_draw_end().y);
        }
        QRectF current_rect((tl-anchor)*m_scale_factor, (br-anchor)*m_scale_factor);
        painter.setPen(Qt::black);
        painter.drawRect(current_rect);
        QPen pen(Qt::white, 1, Qt::DashLine);
        painter.setPen(pen);
        painter.drawRect(current_rect);
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
    Q_UNUSED (event)
    emit current_size(width(), height());
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QPoint scaled_pos = scale_point(event->pos());
    emit mouse_double_click(scaled_pos);
}

/**
 * @brief FrameWidget::mousePressEvent
 * @param event
 */
void FrameWidget::mousePressEvent(QMouseEvent *event) {
    // Pos when the frame is at 100%
    QPoint scaled_pos = scale_point(event->pos());
    switch (m_tool) {
    case NONE:
        break;
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
    case SELECT:
        emit mouse_pressed(scale_point(event->pos()), false);
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
    QPoint scaled_pos = scale_point(event->pos());
    switch (m_tool) {
    case NONE:
        break;
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
    case SELECT:
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
    QPoint scaled_pos = scale_point(event->pos());
    switch (m_tool) {
    case NONE:
        break;
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
            rect_end = scaled_pos;
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
    case SELECT:
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
    QPoint scaled_pos = scale_point(event->pos());
    QPoint num_degree = event->angleDelta() / 8;
    QPoint num_steps = num_degree / 15;
    switch (m_tool) {
    case EDIT:
    case SELECT:
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
                emit center_zoom_rect(scaled_pos, 1/ZOOM_STEP);
            } else {
                emit center_zoom_rect(scaled_pos, ZOOM_STEP);
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

        cv::Point end = cv::Point(ana_rect_end.x(), ana_rect_end.y());
        cv::Point start (ana_rect_start.x(), ana_rect_start.y());
        cv::Rect scaled = cv::Rect(cv::Point(anchor.x()/m_scale_factor + start.x/m_scale_factor, anchor.y()/m_scale_factor + start.y/m_scale_factor),
                      cv::Point(anchor.x()/m_scale_factor + end.x/m_scale_factor, anchor.y()/m_scale_factor + end.y/m_scale_factor));
        settings->set_bounding_box(scaled);

        emit quick_analysis(settings);
        emit set_toolbar_zoom();
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

    // ROI rect points
    int wid = rect_end.x() - rect_start.x();
    int hei = rect_end.y() - rect_start.y();

    double wid_ratio = double(wid) / _qimage.width();
    double height_mod = std::copysign(_qimage.height() * wid_ratio, hei);
    QPoint end = QPoint(rect_end.x(), rect_start.y() + height_mod);

    emit zoom_points(rect_start, end);
}
