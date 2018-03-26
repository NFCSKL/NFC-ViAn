#include "textitem.h"

TextItem::TextItem(Text* text) : ShapeItem(TEXT_ITEM) {
    m_text = text;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, text->get_name());
}

TextItem::~TextItem() {}

void TextItem::remove() {}

void TextItem::rename() {
    m_text->set_name(text(0));
}

Text* TextItem::get_shape() {
    return m_text;
}
