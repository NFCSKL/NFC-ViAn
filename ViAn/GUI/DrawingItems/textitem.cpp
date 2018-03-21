#include "textitem.h"

TextItem::TextItem(Text* text) : ShapeItem(TEXT_ITEM) {
    m_text = text;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    //setText(0, QString::fromStdString(text->get_name()));
    setText(0, m_text->get_text());
}

void TextItem::remove() {}

void TextItem::rename() {
    //m_text->set_name(text(0).toStdString());
    m_text->set_name(m_text->get_text().toStdString());
}

Text* TextItem::get_shape() {
    return m_text;
}
