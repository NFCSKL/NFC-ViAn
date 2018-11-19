#include "tagitem.h"

#include "Project/Analysis/tag.h"

TagItem::TagItem(Tag *tag) : TreeItem(TAG_ITEM) {
    m_tag = tag;
    setText(0, QString::fromStdString(tag->get_name()));
    const QIcon tag_icon("../ViAn/Icons/tag.png");
    setIcon(0, tag_icon);
}

TagItem::~TagItem() {}

void TagItem::remove(){}

void TagItem::rename() {
    m_tag->m_name = text(0).toStdString();
}

Tag *TagItem::get_tag() {
    return m_tag;
}
