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
    void rename();
};

#endif // TEXTITEM_H
