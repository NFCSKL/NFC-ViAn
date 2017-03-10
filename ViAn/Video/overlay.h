#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>
#include <QColor>
#include "shapes.h"
#include <QColor>

class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    void draw_overlay(QImage &img);
    void set_tool(SHAPES shape);
    void set_colour(QColor colour);
    QColor get_colour();
    SHAPES get_shape();
    void mouse_pressed(QPoint pos);
    void mouse_released(QPoint pos);
    void mouse_moved(QPoint pos);

private:
    void update_drawing_position(QPoint pos);

    bool show_overlay = false;
    SHAPES shape = RECTANGLE;
    QColor colour = Qt::red;

    QPoint draw_start = QPoint(400, 200);
    QPoint draw_end = QPoint(800, 600);
};

#endif // OVERLAY_H
