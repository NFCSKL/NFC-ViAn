#include "textitem.h"

TextItem::TextItem(Text* text) : ShapeItem(TEXT_ITEM) {
    m_text = text;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, text->get_name());
    const QIcon text_icon("../ViAn/Icons/text.png");
    setIcon(0, text_icon);

    map = QPixmap(16,16);
    update_shape_color();
    update_show_icon(m_text->get_show());
}

TextItem::~TextItem() {}

void TextItem::remove() {}

Text* TextItem::get_shape() {
    return m_text;
}
