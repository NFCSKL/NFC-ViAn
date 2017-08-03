#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSize>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio/videoio.hpp"

#include "Video/shapes/zoomrectangle.h"
#include "Project/Analysis/analysisproxy.h"
#include "Analysis/analysissettings.h"
enum click_tool {NONE, ZOOM, MOVE,ANALYSIS_BOX};

class FrameWidget : public QWidget
{
    Q_OBJECT
    QPainter* painter;
    QSize m_scroll_area_size;
    cv::Rect original_rect; // Contains the size of the unmodified frame

    std::vector<cv::Rect> ooi_rects;

    click_tool tool = NONE;
    cv::Mat current_frame;
    Analysis* m_analysis = nullptr;
    // Zoom
    QPoint rect_start, rect_end, prev_pos;
    bool mark_rect = false;
    QPoint anchor = QPoint(0,0);

    bool do_zoom = false;
    bool do_zoom_out = false;
    bool m_detections = false;
    bool show_detections = true;

    double m_scale_factor = 1;

public:
    explicit FrameWidget(QWidget *parent = nullptr);

    cv::Mat get_mat() const;

signals:
    void quick_analysis(AnalysisSettings* settings);
    void video_pressed(QPoint pos);
    void video_released(QPoint pos);
    void video_moved(QPoint pos);
    void current_size(int width, int height);
    void moved_xy(int x, int y);
    void current_frame_size(QSize size);
    void zoom_points(QPoint, QPoint);
    void trigger_zoom_out();
public slots:
    void on_new_image(cv::Mat image, int frame_index);
    void toggle_zoom(bool value);
    void set_analysis_tool();
    void set_scroll_area_size(QSize size);
    void set_analysis(AnalysisProxy *);
    void clear_analysis();
    void set_detections_on_frame(int);
    void set_detections(bool);
    void set_show_detections(bool);
    void set_anchor(QPoint);
    void set_scale_factor(double scale_factor);
    void update();
protected:
    QImage _qimage;
    cv::Mat _tmp_frame;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void init_panning(QPoint pos);
    void set_rect_start(QPoint pos);
    void panning(QPoint pos);
    void rect_update(QPoint pos);
    void end_panning();
    void end_zoom();
};

#endif // FRAMEWIDGET_H
