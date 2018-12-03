#include "sequenceitem.h"

#include "Project/imagesequence.h"
#include "Project/videoproject.h"
#include "videoitem.h"

ImageSequence* SequenceItem::get_img_sequence() {
    auto vid_item = dynamic_cast<VideoItem*>(this->parent()->parent());
    if (vid_item) {
        auto sequence = dynamic_cast<ImageSequence*>(vid_item->get_video_project()->get_video());
        if (sequence) {
            return sequence;
        }
    }
    return nullptr;
}



SequenceItem::SequenceItem(const std::string& name, const std::string& hash) : TreeItem(SEQUENCE_ITEM) {
    m_hash = hash;
    setText(0, QString::fromStdString(name));
}

/**
 * @brief SequenceItem::get_index
 * Returns the sequence index of the image
 * @return int index
 */
int SequenceItem::get_index() {
    int index = 0;
    auto sequence = get_img_sequence();
    if (sequence) index = sequence->get_index_of_hash(m_hash);
    return index;
}

/**
 * @brief SequenceItem::get_hash
 * Returns the hash/checksum of the image file belonging to the item
 * @return
 */
std::string SequenceItem::get_hash() const {
    return m_hash;
}

void SequenceItem::remove(){
    ImageSequence* sequence = get_img_sequence();
    if (sequence == nullptr) return; // Something went wrong here TODO
    sequence->remove_image_with_hash(m_hash);
}

void SequenceItem::rename(){}
