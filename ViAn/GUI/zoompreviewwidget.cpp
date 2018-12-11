#include "zoompreviewwidget.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QDebug>
#include <QPainter>
#include <QResizeEvent>

/**
 * @brief zoompreviewwidget::center_image
 * Centers the image in the widget
 * @param s QSize   : The size of the image to be centered
 */
void ZoomPreviewWidget::center_image(const QSize &s) {
    if (center_along_xy) {
        anchor = QPoint((this->width() - s.width()) / 2, (this->height()- s.height()) / 2);
    } else {
        anchor = QPoint((this->width() - s.width()) / 2, 0);
    }
}

ZoomPreviewWidget::ZoomPreviewWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(QSize(200,100));
}

/**
 * @brief zoompreviewwidget::paintEvent
 * Repaint the stored image at anchor point
 * @param event
 */
void ZoomPreviewWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED (event)
    QPainter painter(this);
    painter.drawImage(anchor, _qimage);
}

/**
 * @brief zoompreviewwidget::resizeEvent
 * emits the current window size when the widget is resized
 * @param event
 */
void ZoomPreviewWidget::resizeEvent(QResizeEvent *event) {
    emit window_size(event->size());
}

/**
 * @brief ZoomPreviewWidget::mousePressEvent
 * Event for when the widget is clicked
 * @param event
 */
void ZoomPreviewWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        panning = true;
        start_point = event->pos();
    } else {
        panning = false;
    }
}

/**
 * @brief ZoomPreviewWidget::mouseMoveEvent
 * Event for when the mouse (pressed) is dragged inside the widget
 * @param event
 */
void ZoomPreviewWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!panning) return;
    QPoint movement = event->pos() - start_point;
    start_point = event->pos();
    emit pan_translation(movement);
}

/**
 * @brief zoompreviewwidget::frame_update
 * Converts and stores the provided image as a QImage.
 * This is later used when repainting the widget
 * @param preview_image cv::Mat :   The provided preview image
 */
void ZoomPreviewWidget::frame_update(cv::Mat preview_image) {
    switch (preview_image.type()) {
        case CV_8UC1:
            cvtColor(preview_image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(preview_image, _tmp, CV_BGR2RGB);
            break;
        case CV_8UC4:
            cvtColor(preview_image, _tmp, CV_BGRA2RGB);
            break;
        default:
            qWarning() << "Mat was of unknown type [" << preview_image.type() << "]";
            break;
    }

    assert(_tmp.isContinuous());

    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    QSize s = _qimage.size();
    center_image(s);
    update();
}

/**
 * @brief ZoomPreviewWidget::clean_zoom_widget
 * Clean the zoom preview widget from the last image and paint in to the background color
 */
void ZoomPreviewWidget::clean_zoom_widget() {
    _qimage = QImage(_tmp.cols, _tmp.rows, QImage::Format_RGB32);
    _qimage.fill(QColor(240, 240, 240));
    QSize s = _qimage.size();
    center_image(s);
    update();
}

/**
 * @brief ZoomPreviewWidget::on_floating_changed
 * @param is_floating
 */
void ZoomPreviewWidget::on_floating_changed(const bool is_floating) {
    center_along_xy = is_floating;
}
