#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSize>
#include "Video/shapes/zoomrectangle.h"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "Project/Analysis/analysis.h"

enum click_tool {NONE, ZOOM, MOVE};

class FrameWidget : public QWidget
{
    Q_OBJECT
    QPainter*  painter;
    QSize m_scroll_area_size;
    cv::Rect original_rect; // Contains the size of the unmodified frame

    std::vector<cv::Rect> ooi_rects;

    click_tool tool = NONE;
    cv::Mat current_frame;
    Analysis* m_analysis = nullptr;
    // Zoom
    QPoint zoom_start_pos, zoom_end_pos, prev_pos;
    bool draw_zoom_rect = false;
    bool do_zoom = false;
    bool do_zoom_out = false;
    bool m_detections = false;

public:
    explicit FrameWidget(QWidget *parent = nullptr);

    cv::Mat get_mat() const;

signals:
    void video_pressed(QPoint pos);
    void video_released(QPoint pos);
    void video_moved(QPoint pos);
    void zoom_factor(double scale_factor);
    void current_size(int width, int height);
    void moved_xy(int x, int y);
    void current_frame_size(QSize size);
    void zoom_points(QPoint, QPoint);
    void trigger_zoom_out();
public slots:
    void draw_image(cv::Mat image);
    void draw_from_playback(cv::Mat frame);
    void toggle_zoom(bool value);
    void set_scroll_area_size(QSize size);
    void set_analysis(Analysis*);
    void clear_analysis();
    void set_detections_on_frame(int);
    void set_detections(bool);
protected:
    QImage _qimage;
    cv::Mat _tmp;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void init_panning(QPoint pos);
    void init_zoom(QPoint pos);
    void panning(QPoint pos);
    void zoom(QPoint pos);
    void end_panning();
    void end_zoom();
};

#endif // FRAMEWIDGET_H
