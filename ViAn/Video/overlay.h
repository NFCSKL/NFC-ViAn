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

class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_video_frame_size(int width, int height);
    void set_window_frame_width(int width);
    void set_window_frame_height(int height);
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
    int video_frame_width, video_frame_height;
    int window_frame_width, window_frame_height;
    bool show_overlay = false;
    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
    QString current_string = "";
    std::map<int, QList<shape*>> overlays;
    QList<shape*> drawings;
};

#endif // OVERLAY_H
