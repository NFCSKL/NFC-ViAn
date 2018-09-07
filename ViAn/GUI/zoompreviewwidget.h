#ifndef ZOOMPREVIEWWIDGET_H
#define ZOOMPREVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QResizeEvent>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

class zoompreviewwidget : public QWidget
{
    Q_OBJECT

private:
    cv::Mat _tmp;
    QImage _qimage;


public:
    explicit zoompreviewwidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event);

signals:
    void window_size(QSize s);

public slots:
    void frame_update(cv::Mat frame);
};

#endif // ZOOMPREVIEWWIDGET_H
