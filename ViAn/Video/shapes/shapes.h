#ifndef SHAPES_H
#define SHAPES_H

#include "constants.h"

#include "opencv2/core/core.hpp"

#include <QColor>
#include <QJsonObject>

class QPoint;

enum SHAPES {RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT, EDIT, ZOOM, MOVE, ANALYSIS_BOX};

class Shapes {

public:
    Shapes(SHAPES s);
    Shapes(SHAPES s, QColor col, QPoint pos);
    void set_anchor(QPoint pos);
    void edit_shape(QPoint diff_point);
    void update_drawing_pos(QPoint pos);
    void update_drawing_sym(int dx, int dy);
    void update_text_pos(QPoint pos);
    void update_text_draw_end();
    virtual void move_shape(QPoint p);
    virtual void handle_new_pos(QPoint pos) = 0;
    virtual cv::Mat draw(cv::Mat &frame) = 0;
    virtual cv::Mat draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) = 0;

    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;
    virtual ~Shapes();

    static cv::Scalar qcolor_to_scalar(QColor col);
    static cv::Point qpoint_to_point(QPoint pnt);

    // TODO Remove after analysis area is removed
    static constexpr double ALPHA = 0.6; // Costant used for the opacity.

    cv::Point get_draw_start();
    cv::Point get_draw_end();
    void set_draw_end(cv::Point);
    void set_draw_start(cv::Point);
    SHAPES get_shape();
    QColor get_color();
    void set_color(QColor);
    cv::Size get_text_size();
    void set_text_size(cv::Size size);
    void set_thickness(QPoint pos);
    void set_thickness(int value);
    int get_thickness();
    void set_frame(int);
    int get_frame();
    virtual QString get_name() = 0;
    virtual void set_name(QString name) = 0;
    bool toggle_show();
    bool get_show();

protected:
    SHAPES shape = ZOOM;
    cv::Scalar color;
    QColor q_color;
    int thickness = Singleton::get_instance()->LINE_THICKNESS;
    cv::Point draw_start;
    cv::Point draw_end;
    bool anchor; // true = draw start -- false = draw end; TODO 4 corners
    cv::Size text_size;
    int frame;
    QString m_name = "Unknown shape";
    bool show = true;

    void write_shape(QJsonObject& json);
    void read_shape(const QJsonObject& json);
};

#endif // SHAPES_H
