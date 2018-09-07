#include "zoompreviewwidget.h"
#include <QDebug>


zoompreviewwidget::zoompreviewwidget(QWidget *parent) : QWidget(parent) {}

void zoompreviewwidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
}

void zoompreviewwidget::resizeEvent(QResizeEvent *event) {
    emit window_size(*event->size());
}

void zoompreviewwidget::frame_update(cv::Mat preview_image) {
    switch (preview_image.type()) {
        case CV_8UC1:
            cvtColor(preview_image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(preview_image, _tmp, CV_BGR2RGB);
            break;
        default:
        qDebug("Non handled image type");
            // TODO FIX THIS: SOME TIFF IMAGES HAS AN ALPHA CHANNEL AS WELL
    }

    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());

    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    update();
}


