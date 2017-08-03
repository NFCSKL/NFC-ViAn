#include "bookmarkcategory.h"
#include <QDebug>

BookmarkCategory::BookmarkCategory(std::string name, QListWidget *parent, int type) : QListWidgetItem(parent, type) {
    m_name = name;
    // Setup layout
    QWidget* folder = new QWidget();
    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setMargin(5);
    layout->setSpacing(0);
    folder->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QHBoxLayout* container = new QHBoxLayout();
    m_title = new QLineEdit(QString::fromStdString(name));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(on_text_edited(QString)));
    layout->addWidget(m_title);
    layout->addLayout(container);

    // Add disputed and reference list
    disp_list = new BookmarkList(false, DISPUTED);
    ref_list = new BookmarkList(false, REFERENCE);
    disp_list->set_parent_name(m_name);
    ref_list->set_parent_name(m_name);
    connect(m_title, SIGNAL(textChanged(QString)), disp_list, SLOT(on_parent_name_edited(QString)));
    connect(m_title, SIGNAL(textChanged(QString)), ref_list, SLOT(on_parent_name_edited(QString)));
    connect(disp_list, SIGNAL(set_bookmark_video(VideoProject*,int)), this, SIGNAL(set_bookmark_video(VideoProject*,int)));
    connect(ref_list, SIGNAL(set_bookmark_video(VideoProject*,int)), this, SIGNAL(set_bookmark_video(VideoProject*,int)));

    disputed = make_scrollable_container(disp_list);
    reference = make_scrollable_container(ref_list);

    container->addWidget(disputed);
    container->addWidget(reference);

    folder->setLayout(layout);
    parent->setItemWidget(this, folder);

    setSizeHint(folder->sizeHint());
}

BookmarkCategory::~BookmarkCategory() {
}

std::string BookmarkCategory::get_name() {
    return m_name;
}

std::vector<BookmarkItem *> BookmarkCategory::get_disputed() {
    vector<BookmarkItem *> items;
    for (auto i = 0; i < disp_list->count(); ++i) {
        items.push_back(dynamic_cast<BookmarkItem*>(disp_list->item(i)));
    }
    return items;
}

std::vector<BookmarkItem *> BookmarkCategory::get_references() {
    vector<BookmarkItem *> items;
    for (auto i = 0; i < ref_list->count(); ++i) {
        items.push_back(dynamic_cast<BookmarkItem*>(ref_list->item(i)));
    }
    return items;
}

/**
 * @brief BookmarkCategory::create_menu
 * Creates and returns a QMenu. The calling function is responsible for deleting it
 * @return
 */
QMenu *BookmarkCategory::create_menu(){
    QMenu* menu = nullptr;
}

void BookmarkCategory::update_title(const QString &title){
    m_title->setText(title);
}

/**
 * @brief BookmarkCategory::copy
 * Returns a new BookmarkCategory containing the same Bookmarks
 * @return
 */
BookmarkCategory *BookmarkCategory::copy(QListWidget* new_parent) {
    BookmarkCategory* new_bm_cat = new BookmarkCategory(m_name, new_parent, type());
    // Copy disputed bookmarks
    vector<BookmarkItem*> items = get_disputed();
    std::for_each(items.begin(), items.end(),
                  [new_bm_cat](BookmarkItem* bi){new_bm_cat->add_disp_item(bi->copy());});
    // Copy reference bookmarks
    items = get_references();
    std::for_each(items.begin(), items.end(),
                  [new_bm_cat](BookmarkItem* bi){new_bm_cat->add_ref_item(bi->copy());});
    return new_bm_cat;

}

void BookmarkCategory::add_disp_item(BookmarkItem *bm_item) {
    disp_list->addItem(bm_item);
}

void BookmarkCategory::add_ref_item(BookmarkItem *bm_item) {
    ref_list->addItem(bm_item);
}

QScrollArea *BookmarkCategory::make_scrollable_container(BookmarkList* container) {
    QScrollArea* scroll_area = new QScrollArea();
    scroll_area->setWidget(container);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);
    return scroll_area;
}

/**
 * @brief BookmarkCategory::on_text_edited
 * Updates name with the text from the editbox
 * @param name
 */
void BookmarkCategory::on_text_edited(QString name){
    m_name = name.toStdString();
}


