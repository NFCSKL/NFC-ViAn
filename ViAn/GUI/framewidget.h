#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include "Video/shapes/shapes.h"

#include "opencv2/core/core.hpp"
//#include "opencv2/videoio/videoio.hpp"

#include <QWidget>

class Analysis;
class AnalysisProxy;
class AnalysisSettings;
class VideoProject;

class FrameWidget : public QWidget
{

    Q_OBJECT
    QPainter* painter;
    QSize m_scroll_area_size;

    std::vector<cv::Rect> ooi_rects;
    cv::Rect bounding_box;

    SHAPES m_tool = ZOOM;
    QColor overlay_color = Qt::red;
    Shapes* copied_item = nullptr;
    Analysis* m_analysis = nullptr;
    AnalysisProxy* current_analysis = nullptr;
    VideoProject* m_vid_proj = nullptr;

    // Analysis bounding box
    QPoint ana_rect_start, ana_rect_end;
    bool details_checked = false;
    bool show_box = false;

    // Zoom
    QPoint rect_start, rect_end, prev_pos = QPoint(0,0);
    bool mark_rect = false;
    QPoint anchor = QPoint(0,0);
    QPoint prev_point, prev_point_scaled;
    const int PAN_FACTOR = 20;
    const double ZOOM_STEP = 1.1;

    bool do_zoom = false;
    bool do_zoom_out = false;
    bool m_detections = false;
    bool show_detections = true;
    bool pan_rect = false;
    bool frame_is_clear = false;

    int current_frame_nr = 0;
    double m_scale_factor = 1;
    int m_rotation = 0;

    const int DEGREE_MIN = 0;
    const int DEGREE_90 = 90;
    const int DEGREE_180 = 180;
    const int DEGREE_270 = 270;
    const int DEGREE_MAX = 360;

    std::pair<double, double> panning_tracker {}; // Track when to actually pan. Based on current zoom level

public:
    explicit FrameWidget(QWidget *parent = nullptr);

    cv::Mat get_modified_frame() const;
    cv::Mat get_org_frame() const;
    void set_current_frame_nr(int);
    void clear_frame(bool value);

signals:
    void quick_analysis(AnalysisSettings* settings);
    void video_pressed(QPoint pos);
    void video_released(QPoint pos);
    void video_moved(QPoint pos);
    void current_size(int width, int height);
    void moved_xy(int x, int y);
    void zoom_points(QPoint, QPoint);
    void trigger_zoom_out(double);
    void move_viewport_center(QPoint, double);
    void send_tool(SHAPES m_tool);
    void send_tool_text(QString, float);
    void send_color(QColor color);
    void set_toolbar_zoom();

    void mouse_double_click(QPoint);
    void mouse_pressed(QPoint, bool);
    void mouse_released(QPoint, bool);
    void mouse_moved(QPoint, bool, bool);
    void mouse_scroll(QPoint);
    void process_frame();
public slots:
    void copy();
    void paste();
    void on_new_image(cv::Mat org_image, cv::Mat mod_image, int frame_index);
    void set_details_checked(bool b);
    void set_scroll_area_size(QSize size);
    void set_analysis(AnalysisProxy *);
    void clear_analysis();
    void set_video_project(VideoProject*);
    void set_detections_on_frame(int);
    void set_show_box(bool);
    void set_detections(bool);
    void set_show_detections(bool);
    void set_tool(SHAPES m_tool);
    void set_overlay_color(QColor color);
    void set_anchor(QPoint);
    void set_scale_factor(double scale_factor);
    void set_rotation(int rotation);
    void update();
protected:
    QImage _qimage;
    cv::Mat _tmp_frame;
    cv::Mat m_org_image;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
private:
    void set_cursor(SHAPES m_tool);
    void init_panning(QPoint pos);
    void set_rect_start(QPoint pos);
    void set_analysis_settings();
    void panning(QPoint pos);
    QPoint rotate(QPoint pos, int rotation, bool swap);
    QPoint rect_update(QPoint pos);
    void end_panning();
    void end_zoom();
    QPoint scale_to_video(QPoint pos);
    QPoint scale_to_view(QPoint pos, bool rotate = true);
};

#endif // FRAMEWIDGET_H
