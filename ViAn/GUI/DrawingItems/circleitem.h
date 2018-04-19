#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "shapeitem.h"
#include "Video/shapes/circle.h"

class CircleItem : public ShapeItem {
    Circle* m_circle;
public:
    CircleItem(Circle *circle);
    Circle* get_shape();
    void update_shape_name();
    void update_shape_color();
    void remove();
    ~CircleItem() override;
    QPixmap map;
};

#endif // CIRCLEITEM_H
