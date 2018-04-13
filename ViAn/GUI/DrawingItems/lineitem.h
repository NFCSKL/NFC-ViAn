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
    void remove();
    ~LineItem() override;
};

#endif // LINEITEM_H
