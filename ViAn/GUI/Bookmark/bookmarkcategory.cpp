#include "bookmarkcategory.h"

#include "bookmarkitem.h"
#include "bookmarklist.h"
#include "Project/videoproject.h"

#include <QBoxLayout>
#include <QDebug>
#include <QLineEdit>
#include <QScrollArea>

BookmarkCategory::BookmarkCategory(QString name, int type)
    : BookmarkBaseItem("", type) {
    m_name = name;
    // Setup layout
    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setMargin(5);
    layout->setSpacing(0);
    QHBoxLayout* container = new QHBoxLayout();
    m_title = new QLineEdit(name);
    layout->addWidget(m_title);
    layout->addLayout(container);

    // Add disputed and reference list
    disp_list = new BookmarkList(false, DISPUTED);
    ref_list = new BookmarkList(false, REFERENCE);
    disp_list->set_parent_id(id);
    ref_list->set_parent_id(id);

    // Connect
    connect(m_title, &QLineEdit::textChanged, this, &BookmarkCategory::on_text_edited);
    connect(disp_list, &BookmarkList::set_bookmark_video, this, &BookmarkCategory::set_bookmark_video);
    connect(ref_list, &BookmarkList::set_bookmark_video, this, &BookmarkCategory::set_bookmark_video);

    disputed = make_scrollable_container(disp_list);
    reference = make_scrollable_container(ref_list);

    container->addWidget(disputed);
    container->addWidget(reference);

    folder->setLayout(layout);
    setSizeHint(QSize(Constants::THUMBNAIL_SIZE*3, Constants::THUMBNAIL_SIZE*3));
}

BookmarkCategory::~BookmarkCategory() {
    delete disp_list;
    delete ref_list;
}

void BookmarkCategory::update_index(int index) {
    set_index(index);
    // Update all indexes in the disp and ref list
    disp_list->update_index();
    ref_list->update_index();
    m_unsaved_changes = true;
}

void BookmarkCategory::set_index(int index) {
    m_index = index;
}

void BookmarkCategory::set_project(Project* proj) {
    m_proj = proj;
}

void BookmarkCategory::set_id(const int& new_id) {
    id = new_id;
    disp_list->set_parent_id(new_id);
    ref_list->set_parent_id(new_id);
}

QString BookmarkCategory::get_name() {
    return m_name;
}

Project* BookmarkCategory::get_project() {
    return m_proj;
}

int BookmarkCategory::get_index() {
    return m_index;
}

int BookmarkCategory::get_id() const {
    return id;
}

std::vector<BookmarkItem *> BookmarkCategory::get_disputed() {
    std::vector<BookmarkItem *> items;
    for (auto i = 0; i < disp_list->count(); ++i) {
        items.push_back(dynamic_cast<BookmarkItem*>(disp_list->item(i)));
    }
    return items;
}

std::vector<BookmarkItem *> BookmarkCategory::get_references() {
    std::vector<BookmarkItem *> items;
    for (auto i = 0; i < ref_list->count(); ++i) {
        items.push_back(dynamic_cast<BookmarkItem*>(ref_list->item(i)));
    }
    return items;
}

QWidget* BookmarkCategory::get_folder() {
    return folder;
}

void BookmarkCategory::update_title(const QString &title){
    m_title->setText(title);
    m_unsaved_changes = true;
}

/**
 * @brief BookmarkCategory::copy
 * Returns a new BookmarkCategory containing the same Bookmarks
 * @return
 */
BookmarkCategory *BookmarkCategory::copy() {
    BookmarkCategory* new_bm_cat = new BookmarkCategory(m_name, type());
    // Copy disputed bookmarks
    std::vector<BookmarkItem*> items = get_disputed();
    std::for_each(items.begin(), items.end(),
                  [new_bm_cat, this](BookmarkItem* bi){
        new_bm_cat->add_disp_item(bi->copy());
        bi->get_bookmark()->set_container(id, DISPUTED);
    });

    // Copy reference bookmarks
    items = get_references();
    std::for_each(items.begin(), items.end(),
                  [new_bm_cat, this](BookmarkItem* bi){
        new_bm_cat->add_ref_item(bi->copy());
        bi->get_bookmark()->set_container(id, REFERENCE);
    });
    return new_bm_cat;

}

void BookmarkCategory::add_disp_item(BookmarkItem *bm_item) {
    disp_list->addItem(bm_item);
    disp_list->sortItems();
    m_unsaved_changes = true;
}

void BookmarkCategory::add_ref_item(BookmarkItem *bm_item) {
    ref_list->addItem(bm_item);
    disp_list->sortItems();
    m_unsaved_changes = true;
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
    m_name = name;
    m_unsaved_changes = true;
}

bool BookmarkCategory::is_saved() const {
    return !m_unsaved_changes;
}

void BookmarkCategory::clear_lists() {
    disp_list->clear_lists();
    ref_list->clear_lists();
}
