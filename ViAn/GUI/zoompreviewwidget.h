#ifndef ZOOMPREVIEWWIDGET_H
#define ZOOMPREVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QResizeEvent>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

class ZoomPreviewWidget : public QWidget
{
    Q_OBJECT

private:
    cv::Mat _tmp;
    QImage _qimage;
    QPoint anchor{0, 0};

    bool center_along_xy{false}; // If false the image will only be centered along the x-axis

    void center_image(const QSize& s);

public:
    explicit ZoomPreviewWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event);

signals:
    void window_size(QSize s);

public slots:
    void frame_update(cv::Mat frame);
    void on_floating_changed(const bool is_floating);
};

#endif // ZOOMPREVIEWWIDGET_H
