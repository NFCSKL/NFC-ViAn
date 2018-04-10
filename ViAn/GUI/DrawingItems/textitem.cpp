#include "textitem.h"

TextItem::TextItem(Text* text) : ShapeItem(TEXT_ITEM) {
    m_text = text;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, text->get_name());
    const QIcon text_icon("../ViAn/Icons/text.png");
    setIcon(0, text_icon);

    map = QPixmap(16,16);
    update_shape_color();
}

TextItem::~TextItem() {}

void TextItem::remove() {}

void TextItem::update_shape_name() {
    m_text->set_name(text(0));
}

void TextItem::update_shape_color() {
    map.fill(m_text->get_color());
    setIcon(1, map);
}

Text* TextItem::get_shape() {
    return m_text;
}
