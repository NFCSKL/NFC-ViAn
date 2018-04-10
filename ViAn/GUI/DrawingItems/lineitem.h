#ifndef LINEITEM_H
#define LINEITEM_H

#include "shapeitem.h"
#include "Video/shapes/line.h"

class LineItem : public ShapeItem {
    Line* m_line;
public:
    LineItem(Line* line);
    Line* get_shape();
    void update_shape_name();
    void update_shape_color();
    void remove();
    ~LineItem() override;
    QPixmap map;
};

#endif // LINEITEM_H
