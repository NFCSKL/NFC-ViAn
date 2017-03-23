#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>
#include <QColor>
#include "shapes/shape.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/arrow.h"

class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    void draw_overlay(QImage &img);
    void set_tool(SHAPES s);
    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_shape();
    void mouse_pressed(QPoint pos);
    void mouse_released(QPoint pos);
    void mouse_moved(QPoint pos);

private:
    void update_drawing_position(QPoint pos);

    bool show_overlay = false;
    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
    QList <shape*> drawings;
};

#endif // OVERLAY_H
