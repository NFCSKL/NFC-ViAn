#include "bookmarkwidget.h"

#include "bookmarkcategory.h"
#include "bookmarkitem.h"
#include "bookmarklist.h"
#include "imagegenerator.h"
#include "Project/imagesequence.h"
#include "Project/video.h"
#include "Project/videoproject.h"
#include "reportgenerator.h"

#include <QBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>
#include <QThread>


BookmarkWidget::BookmarkWidget(QWidget *parent) : QWidget(parent) {
    QPushButton* generate_btn = new QPushButton(tr("Generate report"));
    QPushButton* new_folder_btn = new QPushButton(tr("Create new category"));

    bm_list = new BookmarkList(this);
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
    setMinimumWidth(bm_list->sizeHint().width()*2); // Should be 2*thumbnail + margin
    setLayout(layout);

    connect(bm_list, &BookmarkList::set_bookmark_video, this, &BookmarkWidget::play_bookmark_video);
    connect(new_folder_btn, &QPushButton::clicked, bm_list, &BookmarkList::add_new_folder);
    connect(generate_btn, &QPushButton::clicked, this, &BookmarkWidget::generate_report);

    //Context menu for list items
//    bm_list->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(bm_list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(item_context_menu(QPoint)));
}

void BookmarkWidget::generate_report() {
    emit set_status_bar("Generating report. Please wait.");
    emit play_video(false);
    ReportContainer rp_cont;
    std::vector<BookmarkItem*> bmark_list;
    for(int i = 0; i != bm_list->count(); ++i){
        QListWidgetItem* item = bm_list->item(i);
        if (item->type() == CONTAINER) {
            BookmarkCategory* _tmp_cat = dynamic_cast<BookmarkCategory*>(item);
            QString cat_name = QString::fromStdString(_tmp_cat->get_name());

            std::vector<BookmarkItem*> _temp_ref = _tmp_cat->get_references();
            std::vector<BookmarkItem*> _temp_disp = _tmp_cat->get_disputed();

            RefDisp ref_disp = std::make_pair(_temp_disp, _temp_ref);
            rp_cont.push_back(std::make_pair(cat_name, ref_disp));
        } else if (item->type() == BOOKMARK) {
            BookmarkItem* bmark = dynamic_cast<BookmarkItem*>(item);
            bmark_list.push_back(bmark);
        }
    }
    processing_thread = new QThread;
    setCursor(Qt::WaitCursor);
    ReportGenerator* rp_gen = new ReportGenerator(m_path,rp_cont);
    rp_gen->uncat_bmarks = bmark_list;
    rp_gen->create_report();
    setCursor(Qt::ArrowCursor);
}

BookmarkCategory* BookmarkWidget::add_to_container(BookmarkItem *bm_item, std::pair<int, std::string> *container) {
    BookmarkCategory* bm_cat = nullptr;
    for (int i = 0; i < bm_list->count(); i++) {
        QListWidgetItem* item = bm_list->item(i);
        if (item->type() == CONTAINER) {
            BookmarkCategory* _tmp_cat = dynamic_cast<BookmarkCategory*>(item);
            if (_tmp_cat->get_name() == container->second) {
                bm_cat = _tmp_cat;
                break;
            }
        }
    }

    if (bm_cat == nullptr) {
        // Container does not exist. Create and add it
        bm_cat = new BookmarkCategory(container->second, CONTAINER);
        bm_list->addItem(bm_cat);
        bm_list->setItemWidget(bm_cat, bm_cat->get_folder());
        connect(bm_cat, &BookmarkCategory::set_bookmark_video, this, &BookmarkWidget::play_bookmark_video);
    }

    if (container->first == DISPUTED) {
        bm_cat->add_disp_item(bm_item);
    } else {
        bm_cat->add_ref_item(bm_item);
    }
    return bm_cat;
}

void BookmarkWidget::create_bookmark(VideoProject* vid_proj, VideoState state, cv::Mat bookmark_frame, cv::Mat org_frame, QString time, QString description) {
    export_original_frame(vid_proj, state.frame, org_frame);
    std::string file_name;
    if (vid_proj->get_video()->is_sequence()) {
        ImageSequence* seq = dynamic_cast<ImageSequence*>(vid_proj->get_video());
        file_name = Utility::name_from_path(seq->get_original_name_from_index(state.frame));
    } else {
       file_name = vid_proj->get_video()->get_name();
       file_name += "_" + std::to_string(state.frame);
    }

    ImageGenerator im_gen(bookmark_frame, m_path);
    std::string thumbnail_path = im_gen.create_thumbnail(file_name);
    std::string bm_file = im_gen.create_bookmark(file_name);
    Bookmark* bookmark = new Bookmark(vid_proj, bm_file, description.toStdString(), state, time);
    bookmark->set_thumbnail_path(thumbnail_path);
    vid_proj->add_bookmark(bookmark);
    BookmarkItem* bm_item = new BookmarkItem(bookmark, BOOKMARK);
    bm_item->set_thumbnail(thumbnail_path);
    bm_list->addItem(bm_item);

    // If just added the first bookmark
    if (bm_list->count() == 1) {
        emit show_bm_dock(true);
    }
}

void BookmarkWidget::export_original_frame(VideoProject* vid_proj, const int frame_nbr, cv::Mat frame) {
    std::string file_name;
    if (vid_proj->get_video()->is_sequence()) {
        ImageSequence* seq = dynamic_cast<ImageSequence*>(vid_proj->get_video());
        ImageGenerator im_gen(frame, m_path);
        std::string path = seq->get_original_name_from_index(frame_nbr);
        std::string name = Utility::name_from_path(path);
        im_gen.create_tiff(name);
    } else {
        Video* vid = vid_proj->get_video();
        file_name = vid->get_name();
        file_name += "_" + std::to_string(frame_nbr);
        ImageGenerator im_gen(frame, m_path);
        im_gen.create_tiff(file_name);
    }

}

void BookmarkWidget::load_bookmarks(VideoProject *vid_proj) {
    for (auto bm_map : vid_proj->get_bookmarks()) {
        Bookmark* bm = bm_map.second;
        // Load thumbnail TODO add check for file
        std::string t_path = bm->get_thumbnail_path();
        std::pair<int, std::string> new_container = bm->get_container();
        BookmarkItem* bm_item = new BookmarkItem(bm, BOOKMARK);
        bm_item->set_thumbnail(t_path);
        if (new_container.second.empty()) {
            // No container name. Add it to the main list
            bm_list->addItem(bm_item);
        } else {
            add_to_container(bm_item, &new_container);
        }
    }
}

void BookmarkWidget::set_path(std::string path) {
    m_path = path;
}

void BookmarkWidget::clear_bookmarks() {
    bm_list->clear();
}
