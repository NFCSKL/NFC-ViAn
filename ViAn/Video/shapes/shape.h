#ifndef SHAPES_H
#define SHAPES_H

#include <QImage>
#include <qpainter.h>

enum SHAPES {RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT};

class shape {

public:
    shape(QColor col, QPoint pos);
    void update_drawing_pos(QPoint pos);
    virtual void handle_new_pos(QPoint pos) = 0;
    void setup_paint_tool(QPainter &painter);
    virtual void draw(QImage &img) = 0;
protected:
    QColor colour;

    QPoint draw_start;
    QPoint draw_end;
};

#endif // SHAPES_H
