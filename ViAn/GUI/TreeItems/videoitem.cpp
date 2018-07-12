#include "videoitem.h"
#include "sequencecontaineritem.h"

#include "imagegenerator.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "sequenceitem.h"

#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QTreeWidgetItem>

//const QString VideoItem::SEQUENCE_CONTAINER_NAME = "Image";

VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));

    auto vid = m_vid_proj->get_video();
    if (vid && vid->is_sequence()) {
        load_sequence_items();
    } else {
        set_thumbnail();
    }
}

VideoItem::VideoItem(): TreeItem(VIDEO_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0,"placeholder");
}

VideoItem::~VideoItem(){}

void VideoItem::remove(){}

void VideoItem::rename(){}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
}

void VideoItem::set_video_project(VideoProject *vid_proj) {
    m_vid_proj = vid_proj;
    setText(0, QString::fromStdString(vid_proj->get_video()->get_name()));
    load_thumbnail();
}

/**
 * @brief VideoItem::set_thumbnail
 * Create a thumbnail from the video and set it as icon.
 */
void VideoItem::set_thumbnail() {
    std::string path = m_vid_proj->get_video()->file_path;
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap >> frame;
    ImageGenerator im_gen(frame, m_vid_proj->get_proj_path());
    std::string thumbnail_path = im_gen.create_thumbnail(m_vid_proj->get_video()->get_name());
    const QIcon icon(QString::fromStdString(thumbnail_path));
    setIcon(0, icon);
}

/**
 * @brief VideoItem::load_thumbnail
 * Loads the thumbnail path and sets it as icon
 */
void VideoItem::load_thumbnail() {
    std::string path = m_vid_proj->get_proj_path() + Project::THUMBNAIL_FOLDER + m_vid_proj->get_video()->get_name() + ".png";
    const QIcon icon(QString::fromStdString(path));
    setIcon(0, icon);
}

/**
 * @brief VideoItem::load_sequence_items
 * Adds each image from the sequence as a child to the list item.
 * The items are added in order based on their indices
 */
void VideoItem::load_sequence_items() {
    if (m_vid_proj == nullptr ) return;
    auto seq = dynamic_cast<ImageSequence*>(m_vid_proj->get_video());
    if (seq) {
        // Create container item for all SequenceItems
        SequenceContainerItem* container = new SequenceContainerItem();
        addChild(container);

        // Add all SequenceItems
        for (auto pair : seq->get_unsaved_order()) {
            auto seq_item = new SequenceItem(seq->get_original_name_from_hash(pair.first), pair.first);

            // Insert in order
            if (!container->childCount()) {
                // First item to be addded
                container->addChild(seq_item);
            } else {
                // Insert item before items with larger index
                int seq_item_index = seq->get_index_of_hash(pair.first);
                bool added{false};
                for (auto i = 0; i < container->childCount(); ++i) {
                    auto child_item = dynamic_cast<SequenceItem*>(container->child(i));
                    if (child_item) {
                        if (child_item->get_index() > seq_item_index) {
                            container->insertChild(i, seq_item);
                            added = true;
                            break;
                        }
                    }
                }

                if (!added) {
                    // Item should be added at the end
                    container->addChild(seq_item);
                }
            }
        }
        container->update_text();
    }
}
