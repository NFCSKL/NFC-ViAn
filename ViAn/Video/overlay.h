#ifndef OVERLAY_H
#define OVERLAY_H

#include <iostream>
#include <QImage>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QInputDialog>
#include <algorithm>
#include "shapes/shape.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/arrow.h"
#include "shapes/pen.h"
#include "shapes/text.h"
#include "shapes/zoomrectangle.h"
#include "shapes/analysarea.h"
#include "Library/customdialog.h"
#include "opencv2/opencv.hpp"
#include "Filehandler/writeable.h"
/**
 * @brief The FrameOverlay struct
 * used for performing undo/redo operations
 */
struct FrameOverlay{
    // Representing all shapes
    std::vector<Shape*> overlay;
    // Representing the end of drawn shapes
    std::vector<Shape*>::iterator drawn = overlay.end();
};

/**
 * @brief The Overlay class
 * The overlay handles drawing on shapes and it's various operations,
 */
class Overlay : public QObject, public Writeable {
    Q_OBJECT

public slots:
    void set_tool(SHAPES s);
public:
    Overlay();
    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void draw_overlay(cv::Mat &frame, int frame_nr);

    void set_text_settings(QString text, float font_scale);
    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_tool();
    void mouse_pressed(QPoint pos, int frame_nr, bool right_click);
    void mouse_released(QPoint pos, int frame_nr, bool right_click);
    void mouse_moved(QPoint pos, int frame_nr);
    void mouse_scroll(QPoint pos, int frame_nr);
    void update_drawing_position(QPoint pos, int frame_nr);
    void undo(int frame_nr);
    void redo(int frame_nr);
    void clear(int frame_nr);
    void delete_drawing(int frame_nr);
    void clear_overlay();

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    std::map<int, FrameOverlay> get_overlays();
    void set_overlays(std::map<int, FrameOverlay>);

private:
    Shape* get_empty_shape(SHAPES shape_type);
    void empty_undo_list(int frame_nr);
    void add_drawing(Shape *shape, int frame_nr);
    void get_drawing(QPoint pos, int frame_nr);
    bool point_in_drawing(QPoint pos, Shape* shape);
    cv::Point qpoint_to_point(QPoint pnt);
    
    Shape* current_drawing = nullptr;
    QPoint prev_point;
    bool m_right_click = false;

    bool show_overlay = true;
    int baseline = 0;

    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
    QString current_string = "Enter text";
    float current_font_scale = 1;

    std::map<int, FrameOverlay> overlays;
};

#endif // OVERLAY_H
