#ifndef OVERLAY_H
#define OVERLAY_H

#include <QImage>
#include "shapes.h"
#include <QColor>

class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
    void draw_overlay(QImage &img);
    void set_overlay_tool(SHAPES shape);
    void set_overlay_colour(QColor colour);

private:
    bool show_overlay = false;
    SHAPES shape = RECTANGLE;
    QColor colour = Qt::red;
};

#endif // OVERLAY_H
