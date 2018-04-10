#ifndef SHAPES_H
#define SHAPES_H

#include <QImage>
#include <QJsonObject>
#include <QJsonArray>
#include <qpainter.h>
#include <algorithm>

#include "opencv2/opencv.hpp"

enum SHAPES {NONE, RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT, HAND, ZOOM, MOVE, ANALYSIS_BOX};

class Shapes {

public:
    Shapes(SHAPES s);
    Shapes(SHAPES s, QColor col, QPoint pos);
    void set_anchor(QPoint pos);
    void edit_shape(QPoint diff_point);
    void update_drawing_pos(QPoint pos);
    void update_text_pos(QPoint pos);
    void update_text_draw_end();
    virtual void move_shape(QPoint p);
    virtual void handle_new_pos(QPoint pos) = 0;
    virtual cv::Mat draw(cv::Mat &frame) = 0;

    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;
    virtual ~Shapes();

    static cv::Scalar qcolor_to_scalar(QColor col);
    static cv::Point qpoint_to_point(QPoint pnt);

    static const int LINE_THICKNESS = 2; // Constant used for the thickness of the drawn shapes.
    static constexpr double ALPHA = 0.6; // Costant used for the opacity.
    static const int RGB_MAX = 255;      // Constant used for inverting colors.

    cv::Point get_draw_start();
    cv::Point get_draw_end();
    SHAPES get_shape();
    QColor get_color();
    void set_color(QColor);
    void set_text_size(cv::Size size);
    void invert_color();
    void set_thickness(QPoint pos);
    void set_frame(int);
    int get_frame();
    virtual QString get_name() = 0;
    virtual void set_name(QString name) = 0;

protected:
    SHAPES shape = NONE;
    cv::Scalar color;
    QColor q_color;
    int thickness = 2;
    cv::Point draw_start;
    cv::Point draw_end;
    bool anchor; // true = draw start -- false = draw end
    cv::Size text_size;
    bool inverted = false;
    int frame;
    QString m_name = "Unknown shape";

    void write_shape(QJsonObject& json);
    void read_shape(const QJsonObject& json);
};

#endif // SHAPES_H