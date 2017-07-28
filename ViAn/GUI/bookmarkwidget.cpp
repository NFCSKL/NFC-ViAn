#include "bookmarkwidget.h"
#include "Library/customdialog.h"
#include <imagegenerator.h>
#include <QString>
#include <QMenu>

BookmarkWidget::BookmarkWidget(QWidget *parent) : QWidget(parent){

    QPushButton* generate_btn = new QPushButton(tr("Generate"));
    QPushButton* new_folder_btn = new QPushButton(tr("New folder"));
    connect(new_folder_btn, &QPushButton::clicked, this, &BookmarkWidget::add_new_folder);
    connect(generate_btn, &QPushButton::clicked, this, &BookmarkWidget::generate_report);
    bm_list = new BookmarkList(this);
    connect(bm_list, SIGNAL(set_bookmark_video(VideoProject*,int)), this, SIGNAL(play_bookmark_video(VideoProject*,int)));
    bm_list_layout = new QVBoxLayout();

    scroll_area = new QScrollArea();

    scroll_area->setWidget(bm_list);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);

    layout = new QVBoxLayout();    
    layout->setAlignment(Qt::AlignBottom);    
    layout->addWidget(new_folder_btn);
    layout->addWidget(scroll_area);
    layout->addWidget(generate_btn);   
    layout->setMargin(10);
    layout->setSpacing(10);
    setMinimumWidth(bm_list->sizeHint().width()*2); // Should be 2*thumbnail + margin
    setLayout(layout);

    //Context menu for list items
//    bm_list->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(bm_list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(item_context_menu(QPoint)));
}

void BookmarkWidget::add_new_folder() {
    BookmarkCategory* f2 = new BookmarkCategory("name", bm_list, CONTAINER);
    bm_list->addItem(f2);
    connect(f2, SIGNAL(set_bookmark_video(VideoProject*,int)), this, SIGNAL(play_bookmark_video(VideoProject*,int)));

}

void BookmarkWidget::generate_report()
{
    ReportContainer rp_cont;
    for(int i = 0; i != bm_list->count(); ++i){
        QListWidgetItem* item = bm_list->item(i);
        if (item->type() == CONTAINER) {
            BookmarkCategory* _tmp_cat = dynamic_cast<BookmarkCategory*>(item);
            QString cat_name = QString::fromStdString(_tmp_cat->get_name());

            std::vector<BookmarkItem*> _temp_ref = _tmp_cat->get_references();
            std::vector<BookmarkItem*> _temp_disp = _tmp_cat->get_disputed();

            RefDisp ref_disp = std::make_pair(_temp_ref, _temp_disp);
            rp_cont.push_back(std::make_pair(cat_name, ref_disp));
        }
    }
    processing_thread = new QThread;    
    ReportGenerator* rp_gen = new ReportGenerator(m_path,rp_cont);
    rp_gen->create_report();
}

BookmarkCategory* BookmarkWidget::add_to_container(BookmarkItem *bm_item, std::pair<int, string> *container) {
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
        bm_cat = new BookmarkCategory(container->second, bm_list, CONTAINER);
        connect(bm_cat, SIGNAL(set_bookmark_video(VideoProject*,int)), this, SIGNAL(play_bookmark_video(VideoProject*,int)));
    }

    if (container->first == DISPUTED)
        bm_cat->add_disp_item(bm_item);
    else
        bm_cat->add_ref_item(bm_item);
    return bm_cat;
}

void BookmarkWidget::create_bookmark(VideoProject* vid_proj, const int frame_nbr, cv::Mat frame)
{
    bool ok;
    QString text = get_input_text("", &ok);    
    if(!ok) return;

    std::string file_name = vid_proj->get_video()->file_path;
    int index = file_name.find_last_of('/') + 1;
    file_name = file_name.substr(index);
    file_name += "_" + std::to_string(frame_nbr);

    ImageGenerator im_gen(frame, m_path);
    std::string thumbnail_path = im_gen.create_thumbnail(file_name);
    std::string bm_file = im_gen.create_tiff(file_name);
    Bookmark* bookmark = new Bookmark(vid_proj, bm_file, text.toStdString() , frame_nbr);
    vid_proj->add_bookmark(bookmark);

    BookmarkItem* bm_item = new BookmarkItem(bookmark, BOOKMARK);
    bm_item->set_thumbnail(thumbnail_path);
    bm_list->addItem(bm_item);
}


void BookmarkWidget::load_bookmarks(VideoProject *vid_proj) {
    for (auto bm_map : vid_proj->get_bookmarks()) {
        Bookmark* bm = bm_map.second;
        // Load thumbnail TODO add check for file
        std::string t_path = m_path + "_thumbnails/" + vid_proj->get_video()->get_name() + "_" + std::to_string(bm->get_frame_number()) + ".png";
        auto containers = bm->get_containers();
        for (auto it = containers.begin(); it != containers.end(); ++it) {
            // Add bookmark to all its containers
            std::pair<int, std::string> _container = *it;
            BookmarkItem* bm_item = new BookmarkItem(bm, BOOKMARK);
            bm_item->set_thumbnail(t_path);
            if (_container.second.empty()) {
                // No container name. Add it to the main list
                bm_list->addItem(bm_item);
            } else {
                add_to_container(bm_item, &_container);
            }
        }
    }
}


void BookmarkWidget::set_path(string path) {
    m_path = path;
}

void BookmarkWidget::clear_bookmarks() {
    bm_list->clear();
}

/**
 * @brief BookmarkWidget::get_input_text
 * @param bookmark_text Text shown in the text edit when opening the dialog.
 * @param ok Parameter set to false if the user cancels.
 * @return Returns a description for the bookmark,
 *         obtained from the user.
 */
QString BookmarkWidget::get_input_text(std::string text, bool* ok) {
    // Create the dialog
    CustomDialog dialog("Bookmark description", NULL);
    dialog.setWindowFlags(windowFlags() & Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
    dialog.addLabel("Write a description of the bookmark:");
    dialog.addTextEdit(&text, false, false, TEXT_EDIT_MIN_HEIGHT,
                          "Write a description of the bookmark. This will be used when creating a report.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        *ok = false;
        return "";
    }
    *ok = true;
    return QString::fromStdString(text);
}
