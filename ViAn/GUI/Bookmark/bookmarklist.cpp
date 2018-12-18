#include "bookmarklist.h"

#include "bookmarkcategory.h"
#include "bookmarkdialog.h"
#include "bookmarkitem.h"
#include "bookmarkwidget.h"
#include "imagegenerator.h"
#include "Project/project.h"
#include "Project/video.h"
#include "Project/videoproject.h"

#include <QApplication>
#include <QDebug>
#include <QDrag>
#include <QDropEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QShortcut>


BookmarkList::BookmarkList(bool accept_container, int container_type, QWidget* parent) : QListWidget(parent) {
    m_accept_container = accept_container;
    m_list_type = container_type;
    // Enable drag and drop
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setIconSize(QSize(Constants::THUMBNAIL_SIZE, Constants::THUMBNAIL_SIZE));

    // Shortcut for deleting item
    QShortcut* delete_sc = new QShortcut(QKeySequence::Delete, this);
    delete_sc->setContext(Qt::WidgetShortcut);
    connect(delete_sc, &QShortcut::activated, this, &BookmarkList::remove_item);

    // Shortcut for renaming item
    QShortcut* rename_sc = new QShortcut(QKeySequence(Qt::Key_F2), this);
    rename_sc->setContext(Qt::WidgetShortcut);
    connect(rename_sc, &QShortcut::activated, this, &BookmarkList::rename_item);

    clear();
}

BookmarkList::~BookmarkList() {
    for (int i = 0; i < count(); ++i) {
        if (item(i)->type() == BOOKMARK || item(i)->type() == CONTAINER) {
            delete item(i);
        }
    }
    clear();
}

void BookmarkList::clear_lists() {
    for (int i = count()-1; i >= 0; --i) {
        if (item(i)->type() == CONTAINER) {
            BookmarkCategory* cat = dynamic_cast<BookmarkCategory*>(item(i));
            cat->clear_lists();
        } else if (item(i)->type() == BOOKMARK) {
            delete item(i);
        }
    }
}

/**
 * @brief BookmarkList::get_clicked_item
 * @return last clicked item
 */
QListWidgetItem *BookmarkList::get_clicked_item() {
    return clicked_item;
}

/**
 * @brief BookmarkList::set_parent_name
 * Stores the name of the parent container that the widget resides in
 * @param name : name of parent container
 */
void BookmarkList::set_parent_id(const int& new_id) {
    m_par_cont_id = new_id;
}

/**
 * @brief BookmarkList::update_index
 * Update the index on all the items in the bookmarklist
 */
void BookmarkList::update_index() {
    for (int i = 0; i < count(); ++i) {
        QListWidgetItem* item = this->item(i);
        int index = this->row(item);
        switch (item->type()) {
        case CONTAINER: {
            BookmarkCategory* cat_item = dynamic_cast<BookmarkCategory*>(item);
            cat_item->update_index(index);
            break;
        }
        case BOOKMARK: {
            BookmarkItem* b_item = dynamic_cast<BookmarkItem*>(item);
            b_item->get_bookmark()->set_index(index);
            break;
        }
        default:
            break;
        }
    }
}

/**
 * @brief BookmarkList::item_right_clicked
 * Will display the context menu when a bookmark or container is clicked
 * @param pos
 */
void BookmarkList::item_right_clicked(const QPoint pos) {
    QMenu* menu = new QMenu;
    QString rename = (clicked_item->type() == BOOKMARK) ? "Change description" : "Change title";
    menu->addAction(rename, this, &BookmarkList::rename_item);
    menu->addAction("Delete", this, &BookmarkList::remove_item);
    menu->addSeparator();
    if (m_list_type == UNSORTED) {
        menu->addAction("New category", this, [this]{ add_category("New Category");});
    }
    menu->exec(mapToGlobal(pos));
    delete menu;
}

/**
 * @brief BookmarkList::bookmark_drop
 * Creates the new bookmark item and inserts it in the correct index in the bookmark list
 * @param source : The source list where the bookmark was located before. Used to get current item
 * @param event
 */
bool BookmarkList::bookmark_drop(QDropEvent *event) {
    BookmarkList* list = dynamic_cast<BookmarkList*>(event->source());
    BookmarkItem* cast_item = dynamic_cast<BookmarkItem*>(list->currentItem());
    // BookmarkItem. Copy and add
    BookmarkItem* bm_item = cast_item->copy();
    int index = row(itemAt(event->pos()+QPoint(0, Constants::THUMBNAIL_SIZE/2)));
    if (index == -1) {
        index = count();
    }
    bm_item->update_item(index, m_par_cont_id, m_list_type);
    insertItem(index, bm_item);

    event->acceptProposedAction();
    return true;
}

/**
 * @brief BookmarkList::container_drop
 * Creates the new container item and inserts it in the correct index in the bookmark list
 * This new container is an exact copy of the original object
 * @param source : The source list where the container awas located before. Used to get current item
 * @param event
 */
bool BookmarkList::container_drop(QDropEvent *event) {
    // If this list won't accept categories, return
    if (!m_accept_container) {
        event->ignore();
        return false;
    }

    BookmarkList* list = dynamic_cast<BookmarkList*>(event->source());
    BookmarkCategory* cast_item = dynamic_cast<BookmarkCategory*>(list->currentItem());
    BookmarkCategory* cat_item = cast_item->copy();
    int index = row(itemAt(event->pos()+QPoint(0, Constants::THUMBNAIL_SIZE/2)));
    if (index == -1) {
        index = count();
    }
    cat_item->set_index(index);
    insertItem(index, cat_item);

    setItemWidget(cat_item, cat_item->get_folder());
    connect(cat_item, &BookmarkCategory::set_bookmark_video, this, &BookmarkList::set_bookmark_video);

    event->acceptProposedAction();
    return true;
}

/**
 * @brief BookmarkList::rename_item
 * Renames the clicked item
 */
void BookmarkList::rename_item(){
    bool ok;
    switch (currentItem()->type()) {
    case BOOKMARK: {
        auto item = dynamic_cast<BookmarkItem*>(currentItem());
        BookmarkDialog dialog;

        dialog.setTextValue(item->get_bookmark()->get_description());
        dialog.setLabelText("Enter a new description");
        dialog.setWindowTitle("Change description");
        ok = dialog.exec();
        QString new_text = dialog.textValue();
        if (ok) {
            item->update_description(new_text);
        }
        break;
    }
    case CONTAINER: {
        auto item = dynamic_cast<BookmarkCategory*>(currentItem());

        BookmarkDialog dialog;
        dialog.setTextValue(item->get_name());
        dialog.setLabelText("Enter a new category name");
        dialog.setWindowTitle("Category name");
        ok = dialog.exec();
        QString new_text = dialog.textValue();
        if (ok) {
            item->update_title(new_text);
        }
        break;
    }
    default:
        break;
    }
}

/**
 * @brief BookmarkList::remove_item
 * Removes the clicked list item
 */
void BookmarkList::remove_item() {
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Warning);
    msg_box.setMinimumSize(315,125);
    msg_box.setText("Deleting item\n"
                    "This will delete all bookmarks in the categories");
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();
    if (reply == QMessageBox::No) return;

    if (currentItem()->type() == BOOKMARK) {
        BookmarkItem* bm_item = dynamic_cast<BookmarkItem*>(currentItem());
        Bookmark* b_mark = bm_item->get_bookmark();
        b_mark->get_project()->remove_bookmark(b_mark);
        delete bm_item;
    } else if (currentItem()->type() == CONTAINER) {
        BookmarkCategory* cat_item = dynamic_cast<BookmarkCategory*>(currentItem());
        // Remove all bookmarks from the disputed list
        for (auto bm_item : cat_item->get_disputed()) {
            Bookmark* b_mark = bm_item->get_bookmark();
            b_mark->get_project()->remove_bookmark(b_mark);
        }
        // Remove all bookmarks from the references list
        for (auto bm_item : cat_item->get_references()) {
            Bookmark* b_mark = bm_item->get_bookmark();
            b_mark->get_project()->remove_bookmark(b_mark);
        }
        cat_item->get_project()->remove_category(cat_item);
    }
}

/**
 * @brief BookmarkList::mousePressEvent
 * Triggers when the widget is clicked.
 * Will create context menu on right click and
 * setup some value for drop on left click.
 * @param event
 */
void BookmarkList::mousePressEvent(QMouseEvent *event) {
    if (itemAt(event->pos())) {
        clicked_item = itemAt(event->pos());
        setCurrentItem(clicked_item);
        switch (event->button()) {
            case Qt::RightButton:
                // Create context menu
                item_right_clicked(event->pos());
                break;
            case Qt::LeftButton:
                // Drag and drop event
                drag_start_pos = event->pos();
                break;
            default:
            break;
        }

        // TODO Add context menu in the categories.
        // Need to figure out a way to mark the bookmarkcategory as
        // currentItem so remove_item() can remove it.
    } else if (m_list_type != UNSORTED && event->button() == Qt::RightButton) {
//        QMenu* menu = new QMenu;
//        menu->addAction("Delete", this, &BookmarkList::remove_item);
//        menu->exec(mapToGlobal(event->pos()));
//        delete menu;
    } else if (event->button() == Qt::RightButton) {
        QMenu* menu = new QMenu;
        menu->addAction("New category", this, [this]{ add_category("New Category");});
        menu->exec(mapToGlobal(event->pos()));
        delete menu;
    } else {
        setCurrentItem(nullptr);
    }
}

/**
 * @brief BookmarkList::mouseDoubleClickEvent
 * Triggers on mouse double click
 * Will jump to the saved frame and state in the bookmark
 * @param event
 */
void BookmarkList::mouseDoubleClickEvent(QMouseEvent *event) {
    if (!itemAt(event->pos())) return;
    auto item = itemAt(event->pos());
    if (item->type() != BOOKMARK) return;
    BookmarkItem* b_item = dynamic_cast<BookmarkItem*>(item);
    Bookmark* bmark = b_item->get_bookmark();

    // Start video at correct frame
    VideoState state;
    state = bmark->get_state();

    VideoProject* parent = bmark->get_project()->get_video_project(bmark->get_vid_proj_id());
    emit set_bookmark_video(parent, state);
}

/**
 * @brief BookmarkList::mouseReleaseEvent
 * Trigger on releasing the mouse button
 * Used to clear clicked_item which is only used to move items
 * @param event
 */
void BookmarkList::mouseReleaseEvent(QMouseEvent *event) {
    clicked_item = nullptr;
    QListWidget::mouseReleaseEvent(event);
}

/**
 * @brief BookmarkList::mouseMoveEvent
 * Triggers on mouse movement after a press.
 * If the distance is large enough it will start a drop event
 * @param event
 */
void BookmarkList::mouseMoveEvent(QMouseEvent *event) {
    if (clicked_item == nullptr) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - drag_start_pos).manhattanLength()
         < QApplication::startDragDistance())
        return;

    // Add the clicked item type to the mime object
    QDrag *drag = new QDrag(this);
    QList<QListWidgetItem*> items = {clicked_item};
    QByteArray mdata;
    QDataStream stream(&mdata, QIODevice::WriteOnly);
    stream << clicked_item->type();

    QMimeData* mime_data = new QMimeData();
    mime_data->setData(QString("application/x-qabstractitemmodeldatalist"), mdata);
    drag->setMimeData(mime_data);

    // Starts the drop. Will remove the original item if Qt::MoveAction is recieved
    if (event->modifiers() == Qt::ControlModifier) {
        drag->exec(Qt::CopyAction);
    } else {
        drag->exec(Qt::MoveAction);
    }
}

/**
 * @brief BookmarkList::dragEnterEvent
 * Event which is sent to a widget when a drag and drop action enters it
 * @param event
 */
void BookmarkList::dragEnterEvent(QDragEnterEvent *event) {
    // Only accpt the event if the drop item is of the correct type
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        if (event->keyboardModifiers() == Qt::ControlModifier) {
            event->setDropAction(Qt::CopyAction);
        } else {
            event->setDropAction(Qt::MoveAction);
        }
        event->accept();
        QListWidget::dragEnterEvent(event);
    } else {
        event->ignore();
    }
}

/**
 * @brief BookmarkList::dropEvent
 * This function is triggered when a listwidgetitem is dropped.
 * It will create a copy of the item and add it to the list.
 * It is up to the source listwidget to remove the original item.
 * @param event
 */
void BookmarkList::dropEvent(QDropEvent *event) {
    // Get subclass type from mimedata
    const QMimeData* mime_data = event->mimeData();
    QByteArray encoded = mime_data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    int type;
    stream >> type;

    // Get origin widget of drag item
    bool success = false;
    if (type == BOOKMARK) {
        success = bookmark_drop(event);
    } else if (type == CONTAINER) {
        success = container_drop(event);
    }

    if (success) {
        BookmarkList* list = dynamic_cast<BookmarkList*>(event->source());
        if (event->dropAction() == Qt::MoveAction) {
            delete list->takeItem(list->row(list->clicked_item));
        }
        list->clicked_item = nullptr;
    }

    update_index();
    sortItems();
}
