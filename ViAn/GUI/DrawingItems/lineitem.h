#ifndef LINEITEM_H
#define LINEITEM_H

#include "shapeitem.h"
#include "Video/shapes/line.h"

class LineItem : public ShapeItem {
    Line* m_line;
public:
    LineItem(Line* line);
    Line* get_shape();
    void rename();
    void remove();
    ~LineItem() override;
};

#endif // LINEITEM_H
