#include "bookmarkwidget.h"

#include "bookmarkcategory.h"
#include "bookmarkitem.h"
#include "bookmarklist.h"
#include "imagegenerator.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/video.h"
#include "Project/videoproject.h"
#include "reportgenerator.h"
#include "utility.h"

#include <QBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>
#include <QThread>


BookmarkWidget::BookmarkWidget(QWidget *parent) : QWidget(parent) {
    QPushButton* generate_btn = new QPushButton(tr("Generate report"));
    QPushButton* new_folder_btn = new QPushButton(tr("Create new category"));

    bm_list = new BookmarkList(true, UNSORTED, this);
    scroll_area = new QScrollArea();

    scroll_area->setWidget(bm_list);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);

    layout = new QVBoxLayout();    
    layout->setAlignment(Qt::AlignBottom);    
    layout->addWidget(new_folder_btn);
    layout->addWidget(scroll_area);
    layout->addWidget(generate_btn);   
    layout->setMargin(5);
    layout->setSpacing(5);
    setMinimumWidth(Constants::THUMBNAIL_SIZE*3);
    setLayout(layout);

    connect(bm_list, &BookmarkList::set_bookmark_video, this, &BookmarkWidget::play_bookmark_video);
    connect(bm_list, &BookmarkList::add_category, this, &BookmarkWidget::add_new_folder);
    connect(new_folder_btn, &QPushButton::clicked, this, [this]{ add_new_folder("New Category");});
    connect(generate_btn, &QPushButton::clicked, this, &BookmarkWidget::generate_report);
}

void BookmarkWidget::generate_report() {
    emit set_status_bar("Generating report. Please wait.");
    emit play_video(false);
    std::vector<QListWidgetItem*> list_items;
    for(int i = 0; i != bm_list->count(); ++i){
        list_items.push_back(bm_list->item(i));
    }
    processing_thread = new QThread;
    setCursor(Qt::WaitCursor);
    ReportGenerator* rp_gen = new ReportGenerator(m_proj->get_dir(), list_items);
    rp_gen->create_report();
    setCursor(Qt::ArrowCursor);
}

/**
 * @brief BookmarkWidget::add_new_folder
 * Add a new category to the list with the name "name"
 * @param name
 * @return
 */
BookmarkCategory* BookmarkWidget::add_new_folder(QString name) {
    BookmarkCategory* new_category = new BookmarkCategory(name, CONTAINER);
    bm_list->addItem(new_category);
    bm_list->setItemWidget(new_category, new_category->get_folder());
    m_proj->add_category(new_category);
    connect(new_category, &BookmarkCategory::set_bookmark_video, this, &BookmarkWidget::play_bookmark_video);
    bm_list->update_index();
    return new_category;
}

void BookmarkWidget::add_to_container(BookmarkItem *bm_item, int type) {
    for (BookmarkCategory* cat : m_proj->get_categories()) {
        Bookmark* bmark = bm_item->get_bookmark();
        if (cat->get_id() == bmark->get_container_id()) {
            if (type == DISPUTED) {
                cat->add_disp_item(bm_item);
            } else if (type == REFERENCE) {
                cat->add_ref_item(bm_item);
            }
            return;
        }
    }
}

void BookmarkWidget::create_bookmark(VideoProject* vid_proj, VideoState state, cv::Mat bookmark_frame, cv::Mat org_frame, QString time, QString description) {
    QString export_path = export_original_frame(vid_proj, state.frame, org_frame);
    // Get the original file name
    // Videos and sequences is saved differently so need to get them different ways
    QString file_name;
    if (vid_proj->get_video()->is_sequence()) {
        ImageSequence* seq = dynamic_cast<ImageSequence*>(vid_proj->get_video());
        file_name = Utility::name_from_path(seq->get_original_name_from_index(state.frame));
    } else {
        file_name = vid_proj->get_video()->get_name();
        file_name += "_" + QString::number(state.frame);
    }

    // Generate the bookmark and thumbnail file
    ImageGenerator im_gen(bookmark_frame, m_proj->get_dir());
    QString thumbnail_path = im_gen.create_thumbnail(file_name);
    QString bm_file = im_gen.create_bookmark(file_name);

    Bookmark* bookmark = new Bookmark(bm_file, description, state, time);
    bookmark->m_image_name = Utility::name_from_path(export_path);
    bookmark->set_thumbnail_path(thumbnail_path);
    bookmark->set_vid_proj_id(vid_proj->get_id());

    m_proj->add_bookmark(bookmark);

    BookmarkItem* bm_item = new BookmarkItem(bookmark, BOOKMARK);
    bm_item->set_thumbnail(thumbnail_path);
    bm_list->addItem(bm_item);
    bm_list->update_index();

    // If just added the first bookmark
    if (bm_list->count() == 1) {
        emit show_bm_dock(true);
    }
}

QString BookmarkWidget::export_original_frame(VideoProject* vid_proj, const int frame_nbr, cv::Mat frame) {
    QString file_name;
    QString export_path;
    if (vid_proj->get_video()->is_sequence()) {
        ImageSequence* seq = dynamic_cast<ImageSequence*>(vid_proj->get_video());
        ImageGenerator im_gen(frame, m_proj->get_dir());
        QString path = seq->get_original_name_from_index(frame_nbr);
        QString name = Utility::name_from_path(path);
        export_path = im_gen.create_tiff(name);
    } else {
        Video* vid = vid_proj->get_video();
        file_name = vid->get_name();
        file_name += "_" + QString::number(frame_nbr);
        ImageGenerator im_gen(frame, m_proj->get_dir());
        export_path = im_gen.create_tiff(file_name);
    }
    return export_path;
}

void BookmarkWidget::load_bookmarks() {
    // Insert all categories on the correct index
    for (BookmarkCategory* cat : m_proj->get_categories()) {
        int index = cat->get_index();
        bm_list->insertItem(index, cat);
        bm_list->setItemWidget(cat, cat->get_folder());
        connect(cat, &BookmarkCategory::set_bookmark_video, this, &BookmarkWidget::play_bookmark_video);
    }

    for (Bookmark* bm : m_proj->get_bookmarks()) {
        // Load thumbnail TODO add check for file
        QString t_path = bm->get_thumbnail_path();
        BookmarkItem* bm_item = new BookmarkItem(bm, BOOKMARK);
        bm_item->set_thumbnail(t_path);

        switch (bm->get_type()) {
        case UNSORTED:
            bm_list->addItem(bm_item);
            break;
        case DISPUTED:
            add_to_container(bm_item, DISPUTED);
            break;
        case REFERENCE:
            add_to_container(bm_item, REFERENCE);
            break;
        }
    }
    bm_list->sortItems();
}

void BookmarkWidget::save_item_data() {
    bm_list->update_index();
}

void BookmarkWidget::set_project(Project* proj) {
    m_proj = proj;
    bm_list->clear_lists();
    load_bookmarks();
}

void BookmarkWidget::clear_bookmarks() {
    bm_list->clear();
}
