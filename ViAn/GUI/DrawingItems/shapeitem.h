#ifndef SHAPEITEM_H
#define SHAPEITEM_H
#include <QTreeWidgetItem>
#include "Video/shapes/shapes.h"

enum DRAWING_ITEM {FRAME_ITEM = 2001, RECT_ITEM, CIRCLE_ITEM, LINE_ITEM, ARROW_ITEM, PEN_ITEM, TEXT_ITEM};
class ShapeItem : public QTreeWidgetItem {
public:
    ShapeItem(int type);
    virtual ~ShapeItem();
    virtual void remove() = 0;
    virtual void update_shape_name() = 0;
    virtual Shapes* get_shape() = 0;
};

#endif // SHAPEITEM_H
