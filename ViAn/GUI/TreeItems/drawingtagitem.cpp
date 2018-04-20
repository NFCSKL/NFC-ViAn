#include "drawingtagitem.h"

DrawingTagItem::DrawingTagItem(DrawingTag *tag) : TreeItem(DRAWING_TAG_ITEM) {
    m_tag = tag;
    setText(0, QString::fromStdString(tag->get_name()));
    const QIcon folder_icon("../Vian/Icons/pen.png");
    setIcon(0, folder_icon);
}

void DrawingTagItem::remove() {}

void DrawingTagItem::rename() {
    m_tag->m_name = text(0).toStdString();
}

DrawingTag *DrawingTagItem::get_tag() {
    return m_tag;
}
