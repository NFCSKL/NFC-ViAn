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
    void update_shape_name();
    void update_shape_color();
    virtual Shapes* get_shape() = 0;
    QPixmap map;
};

#endif // SHAPEITEM_H
