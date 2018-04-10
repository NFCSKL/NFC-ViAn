#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "shapeitem.h"
#include "Video/shapes/text.h"

class TextItem : public ShapeItem {
    Text* m_text = nullptr;
public:
    TextItem(Text* text);
    Text* get_shape();
    void remove();
    void update_shape_name();
    void update_shape_color();
    ~TextItem() override;
    QPixmap map;
};

#endif // TEXTITEM_H
