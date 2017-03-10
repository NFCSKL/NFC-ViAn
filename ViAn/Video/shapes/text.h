#ifndef TEXT_H
#define TEXT_H

#include "shape.h"

class text : public shape {
public:
    text(QColor col, QPoint pos, QString strng);
    void draw(QImage &img) override;
    void add_drawing_pos(QPoint pos);
private:
    QString string;
};

#endif // TEXT_H
