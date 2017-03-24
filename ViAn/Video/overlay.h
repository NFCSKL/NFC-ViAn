#ifndef OVERLAY_H
#define OVERLAY_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

#include <QImage>
#include <QColor>
#include "shapes/shape.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/arrow.h"
#include "shapes/pen.h"
<<<<<<< HEAD
#include "shapes/zoomrectangle.h"
=======
#include "shapes/text.h"
>>>>>>> overlay-scale-export

class overlay {

public:
    overlay();

    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void toggle_overlay();
<<<<<<< HEAD
    cv::Mat draw_overlay(cv::Mat &frame);
    void draw_overlay(QImage &img);
=======
    void draw_overlay(QImage &img, int frame_nr);
>>>>>>> overlay-scale-export
    void set_tool(SHAPES s);
    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_shape();
<<<<<<< HEAD
    void mouse_pressed(QPoint pos);
    void mouse_released(QPoint pos);
    void mouse_moved(QPoint pos);
    void undo();
    void clear();
    void zoom_in();
    void zoom_out();
=======
    void mouse_pressed(QPoint pos, int frame_nr);
    void mouse_released(QPoint pos, int frame_nr);
    void mouse_moved(QPoint pos, int frame_nr);
    void undo(int frame_nr);
    void clear(int frame_nr);
>>>>>>> overlay-scale-export

private:
    void update_drawing_position(QPoint pos, int frame_nr);

    bool show_overlay = false;
    bool choosing_zoom_area = false;

    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
<<<<<<< HEAD
    QList<shape*> drawings;

    zoomrectangle* zoom_area = new zoomrectangle();
=======
    QString current_string = "";
    std::map<int, QList<shape*>> overlays;
>>>>>>> overlay-scale-export
};

#endif // OVERLAY_H
