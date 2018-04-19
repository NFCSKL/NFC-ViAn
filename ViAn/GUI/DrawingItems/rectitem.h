#ifndef RECTITEM_H
#define RECTITEM_H

#include "shapeitem.h"
#include "Video/shapes/rectangle.h"

class RectItem : public ShapeItem {
    Rectangle* m_rect;
public:
    RectItem(Rectangle* rect);
    Rectangle* get_shape();
    void update_shape_name();
    void update_shape_color();
    void remove();
    ~RectItem() override;
    QPixmap map;
};

#endif // RECTITEM_H
