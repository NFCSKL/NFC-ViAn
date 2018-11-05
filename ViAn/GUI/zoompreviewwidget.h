#ifndef ZOOMPREVIEWWIDGET_H
#define ZOOMPREVIEWWIDGET_H

#include <QWidget>

#include "opencv2/core/core.hpp"

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
    void resizeEvent(QResizeEvent *event) override;

signals:
    void window_size(QSize s);

public slots:
    void frame_update(cv::Mat frame);
    void clean_zoom_widget();
    void on_floating_changed(const bool is_floating);
};

#endif // ZOOMPREVIEWWIDGET_H
