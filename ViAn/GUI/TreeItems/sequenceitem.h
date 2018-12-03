#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include "treeitem.h"
#include "videoitem.h"
#include "Project/imagesequence.h"

class SequenceItem : public TreeItem {
private:
    std::string m_hash{};

    ImageSequence* get_img_sequence();
public:
    SequenceItem(const std::string& name, const std::string& hash);
    int get_index();
    std::string get_hash() const;
    void remove();
    void rename();
};

#endif // SEQUENCEITEM_H
