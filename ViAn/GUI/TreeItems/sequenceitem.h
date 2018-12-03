#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include "treeitem.h"

class ImageSequence;
class SequenceItem : public TreeItem {
private:
    QString m_hash{};

    ImageSequence* get_img_sequence();
public:
    SequenceItem(const QString& name, const QString& hash);
    int get_index();
    QString get_hash() const;
    void remove();
    void rename();
};

#endif // SEQUENCEITEM_H
