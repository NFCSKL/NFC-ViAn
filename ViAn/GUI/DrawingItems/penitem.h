#ifndef PENITEM_H
#define PENITEM_H

#include "shapeitem.h"
#include "Video/shapes/pen.h"

class PenItem : public ShapeItem {
    Pen* m_pen;
public:
    PenItem(Pen* pen);
    Pen* get_shape();
    void update_shape_name();
    void update_shape_color();
    void remove();
    ~PenItem() override;
    QPixmap map;
};

#endif // PENITEM_H
