#ifndef PEN_H
#define PEN_H

#include "Shape.h"

class Pen : public Shape {
public:
    Pen(QColor col, QPoint pos);
    void draw(QImage &img) override;
    void handle_new_pos(QPoint pos);
private:
    QVector<QLine> lines;
};

#endif // PEN_H
