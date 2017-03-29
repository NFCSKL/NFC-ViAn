#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>
#include <QColor>
#include "shapes/Shape.h"
#include "shapes/Rectangle.h"
#include "shapes/Circle.h"
#include "shapes/Line.h"
#include "shapes/Arrow.h"
#include "shapes/Pen.h"
#include "shapes/Text.h"
#include "shapes/ZoomRectangle.h"

class Overlay {

public:
    Overlay();
    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    void draw_overlay(QImage &img, int frame_nr);
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

    std::map<int, QList<Shape*>> overlays;
};

#endif // OVERLAY_H
