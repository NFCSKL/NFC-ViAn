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
#include "shapes/shapes.h"
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
 * @brief The Overlay class
 * The overlay handles drawing on shapes and it's various operations,
 */
class Overlay : public QObject, public Writeable {
    Q_OBJECT
    bool m_unsaved_changes = true;
public slots:
    void set_tool(SHAPES s);
public:
    Overlay();
    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void draw_overlay(cv::Mat &frame, int frame_nr);

    void create_text(QPoint, int);
    void set_text_settings(QString text, float font_scale);
    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_tool();
    void mouse_pressed(QPoint pos, int frame_nr, bool right_click);
    void mouse_released(QPoint pos, int frame_nr, bool right_click);
    void mouse_moved(QPoint pos, int frame_nr);
    void mouse_scroll(QPoint pos, int frame_nr);
    void update_drawing_position(QPoint pos, int frame_nr);
    void update_text(QString, Shapes*shape);
    void clear(int frame_nr);
    void delete_drawing(Shapes *shape);
    void clear_overlay();

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    std::map<int, std::vector<Shapes*>> get_overlays();
    void set_overlays(std::map<int, std::vector<Shapes*>>);
    void set_current_drawing(Shapes* shape);
    Shapes* get_current_drawing();
    bool get_show_overlay();

    void set_overlay_changed();
    bool is_saved() const;

private:
    Shapes* get_empty_shape(SHAPES shape_type);
    void add_drawing(Shapes *shape, int frame_nr);
    void get_drawing(QPoint pos, int frame_nr);
    bool point_in_drawing(QPoint pos, Shapes* shape);
    cv::Point qpoint_to_point(QPoint pnt);
    
    Shapes* current_drawing = nullptr;
    QPoint prev_point;
    bool m_right_click = false;

    bool show_overlay = true;
    int baseline = 0;

    SHAPES current_shape = NONE;
    QColor current_colour = Qt::red;
    QString current_string = "Enter text";
    float current_font_scale = 1;

    std::map<int, std::vector<Shapes*>> overlays;

signals:
    void new_drawing(Shapes* shape, int frame_nr);
    void select_current(Shapes*, int);
};

#endif // OVERLAY_H
