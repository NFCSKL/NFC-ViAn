#ifndef RECTITEM_H
#define RECTITEM_H

#include "shapeitem.h"
#include "Video/shapes/rectangle.h"

class RectItem : public ShapeItem {
    Rectangle* m_rect;
public:
    RectItem(Rectangle* rect);
    Rectangle* get_shape() override;
    void remove() override;
    ~RectItem() override;
};

#endif // RECTITEM_H
