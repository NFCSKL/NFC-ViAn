#ifndef ARROWITEM_H
#define ARROWITEM_H

#include "shapeitem.h"
#include "Video/shapes/arrow.h"

class ArrowItem : public ShapeItem {
    Arrow* m_arrow;
public:
    ArrowItem(Arrow* arrow);
    ~ArrowItem() override;
    Arrow* get_shape();
    void update_shape_name();
    void remove();
};

#endif // ARROWITEM_H
