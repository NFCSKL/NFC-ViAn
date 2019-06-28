#include "tagitem.h"

#include "Project/Analysis/tag.h"

TagItem::TagItem(Tag *tag) : TreeItem(TAG_ITEM) {
    m_tag = tag;
    setText(0, tag->get_name());
    QString path = ":/Icons/tag.png";
    if (m_tag->get_type() == SEQUENCE_TAG) {
        path = ":/Icons/image_sequence.png";
    }
    const QIcon tag_icon(path);
    setIcon(0, tag_icon);
}

TagItem::~TagItem() {}

void TagItem::remove(){}

void TagItem::rename() {
    m_tag->m_name = text(0);
}

Tag *TagItem::get_tag() {
    return m_tag;
}
