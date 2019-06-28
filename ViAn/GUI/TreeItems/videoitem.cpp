#include "videoitem.h"

#include "imagegenerator.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "sequencecontaineritem.h"
#include "sequenceitem.h"
#include "utility.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QTreeWidgetItem>
#include <QDebug>

VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, video_project->get_video()->get_name());

    auto vid = m_vid_proj->get_video();
    if (vid && vid->is_sequence()) {
        load_sequence_items();
    }
    set_thumbnail();
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
    setText(0, vid_proj->get_video()->get_name());
    load_thumbnail();
}

/**
 * @brief VideoItem::set_thumbnail
 * Create a thumbnail from the video and set it as icon.
 */
void VideoItem::set_thumbnail() {
    std::string path;
    if (m_vid_proj->get_video()->is_sequence()) {
        ImageSequence* img_seq = dynamic_cast<ImageSequence*>(m_vid_proj->get_video());
        path = img_seq->get_original_name_from_index(0).toStdString();
    } else {
        path = m_vid_proj->get_video()->file_path.toStdString();
    }

    // NIAP This one is not created when path is a .png file
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) {
        setIcon(0, error_icon);
        return;
    }
    cv::Mat frame;
    cap >> frame;
    switch (frame.type()) {
        case CV_8UC1:
            cvtColor(frame, frame, cv::COLOR_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            break;
    }
    ImageGenerator im_gen(frame, m_vid_proj->get_proj_path());
    QString thumbnail_path = im_gen.create_thumbnail(m_vid_proj->get_video()->get_name());
    const QIcon icon(thumbnail_path);
    setIcon(0, icon);
}

/**
 * @brief VideoItem::load_thumbnail
 * Loads the thumbnail path and sets it as icon
 */
void VideoItem::load_thumbnail() {
    QString path = m_vid_proj->get_proj_path() + Constants::THUMBNAIL_FOLDER + m_vid_proj->get_video()->get_name() + ".png";
    const QIcon icon(path);
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
    if (seq && seq->get_sequence_type() == VIDEO_SEQUENCE) {
        // Create container item for all SequenceItems
        SequenceContainerItem* container = new SequenceContainerItem();
        addChild(container);

        // Add all SequenceItems
        for (auto pair : seq->get_unsaved_order()) {
            auto seq_item = new SequenceItem(Utility::name_from_path(seq->get_original_name_from_hash(pair.first)), pair.first);

            // Add thumbnail
            // NIAP
            // TODO!! Move this to a seperate thread
            std::string path = seq->get_original_name_from_hash(pair.first).toStdString();
            cv::VideoCapture cap(path);
            cv::Mat frame;
            bool error = false;
            if (!cap.isOpened()) {
                error = true;
            }
            else {
                cap >> frame;
                switch (frame.type()) {
                    case CV_8UC1:
                        cvtColor(frame, frame, cv::COLOR_GRAY2RGB);
                        break;
                    case CV_8UC3:
                        cvtColor(frame, frame, cv::COLOR_BGR2RGB);
                        break;
                }
            }


            // Check if thumbnail already exists
            // then open it instead of creating new one
            // NIAP Check here for thumbnails path if they exist
            QString name = m_vid_proj->get_video()->get_name();
            int index = seq->get_index_of_hash(pair.first);
            name = name + "-" + QString::number(index);
            QString thumb_path = m_vid_proj->get_proj_path() + Constants::THUMBNAIL_FOLDER +
                    name + ".png";
            QFile file(thumb_path);
            if (error) {
                seq_item->setIcon(0, error_icon);
            } else if (file.exists()) {
                const QIcon icon(thumb_path);
                seq_item->setIcon(0, icon);
            } else {
                ImageGenerator im_gen(frame, m_vid_proj->get_proj_path());
                QString thumbnail_path = im_gen.create_thumbnail(name);
                const QIcon icon(thumbnail_path);
                seq_item->setIcon(0, icon);
            }

            // Insert in order
            if (!container->childCount()) {
                // First item to be added
                container->addChild(seq_item);
            } else {
                // Insert item before items with larger index
                int seq_item_index = seq->get_index_of_hash(pair.first);
                bool added{false};
                for (auto i = 0; i < container->childCount(); ++i) {
                    auto child_item = dynamic_cast<SequenceItem*>(container->child(i));
                    if (!child_item) {
                        continue;
                    } else if (child_item->get_index() > seq_item_index) {
                        container->insertChild(i, seq_item);
                        added = true;
                        break;
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
