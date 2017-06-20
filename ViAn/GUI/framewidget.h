#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

class FrameWidget : public QWidget
{
    Q_OBJECT
    bool do_update = false;
public:
    explicit FrameWidget(QWidget *parent = nullptr);

signals:

public slots:
    void draw_from_playback(cv::Mat frame);
    void draw_image(cv::Mat image);
    void accept_update();
    void block_update();
protected:
    QImage _qimage;
    cv::Mat _tmp;

    void paintEvent(QPaintEvent *event) override;
};

#endif // FRAMEWIDGET_H
