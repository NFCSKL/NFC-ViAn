#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>
#include <QColor>
#include "shapes/shape.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/arrow.h"
#include "shapes/pen.h"
#include "shapes/text.h"
#include "shapes/zoomrectangle.h"

#include "opencv2/opencv.hpp"

class overlay {

public:
    overlay();
    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    cv::Mat draw_overlay(cv::Mat &frame, int frame_nr);
    void set_tool(SHAPES s);
    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_shape();
    void mouse_pressed(QPoint pos, int frame_nr);
    void mouse_released(QPoint pos, int frame_nr);
    void mouse_moved(QPoint pos, int frame_nr);
    void undo(int frame_nr);
    void clear(int frame_nr);

private:
    void update_drawing_position(QPoint pos, int frame_nr);

    bool show_overlay = false;

    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
    QString current_string = "";

    std::map<int, QList<shape*>> overlays;
};

#endif // OVERLAY_H
