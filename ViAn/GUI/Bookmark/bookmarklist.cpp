#include "bookmarkwidget.h"
#include "bookmarkitem.h"
#include "bookmarklist.h"
#include "imagegenerator.h"
#include "bookmarkdialog.h"
#include <QAction>
#include <QMenu>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <algorithm>
#include <QDebug>

BookmarkList::BookmarkList(bool accept_container, int container_type, QWidget* parent) : QListWidget(parent) {
    m_accept_container = accept_container;
    m_container_type = container_type;
    // Enable drag and drop
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setIconSize(QSize(ImageGenerator::THUMBNAIL_SIZE, ImageGenerator::THUMBNAIL_SIZE));

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

void BookmarkList::add_new_folder() {
    BookmarkCategory* new_category = new BookmarkCategory(std::string("Category " +  std::to_string(category_cnt++)), CONTAINER);
    addItem(new_category);
    setItemWidget(new_category, new_category->get_folder());
    connect(new_category, &BookmarkCategory::set_bookmark_video, this, &BookmarkList::set_bookmark_video);
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
void BookmarkList::set_parent_name(std::string& name) {
    m_par_cont_name = name;
}

/**
 * @brief BookmarkList::get_parent_name
 * Returns the name of the parent container that the widget resides in.
 * @return m_par_cont_name : name of parent container
 */
std::string BookmarkList::get_parent_name() {
    return m_par_cont_name;
}

/**
 * @brief BookmarkList::on_parent_name_edited
 * Stores the updated parent container name in each bookmark
 * @param name
 */
void BookmarkList::on_parent_name_edited(QString name) {
    for (auto i = 0; i < this->count(); ++i) {
        auto item = this->item(i);
        if (item->type() == 0) {
            // BookmarkItem. Update container name
            auto bm_item = dynamic_cast<BookmarkItem*>(item);
            bm_item->get_bookmark()->rename_container(m_par_cont_name, name.toStdString());
        }
    }
    m_par_cont_name = name.toStdString();
}

/**
 * @brief BookmarkList::item_right_clicked
 * Will display the context menu when a bookmark or container is clicked
 * @param pos
 */
void BookmarkList::item_right_clicked(const QPoint pos) {
    QMenu* menu = new QMenu;
    QString rename = (clicked_item->type() == BOOKMARK) ? "Change description" : "Change title";
    menu->addAction(rename, this, SLOT(rename_item()));
    menu->addAction("Delete", this, SLOT(remove_item()));
    menu->addSeparator();
    if (m_container_type == UNSORTED) {
        menu->addAction("New category", this, SLOT(add_new_folder()));
    }
    menu->exec(mapToGlobal(pos));
    delete menu;
}

/**
 * @brief BookmarkList::bookmark_drop
 * Creates the new bookmark item and inserts it in the correct index in the bookmark list
 * @param source : The source list where the bookmark awas located before. Used to get current item
 * @param event
 */
void BookmarkList::bookmark_drop(BookmarkList *source, QDropEvent *event) {
    // BookmarkItem. Copy and add
    auto item = source->currentItem();
    auto cast_item = dynamic_cast<BookmarkItem*>(item);

    BookmarkItem* bm_item = cast_item->copy();
    bm_item->get_bookmark()->set_container(m_par_cont_name, m_container_type);
    int index = row(itemAt(event->pos()+QPoint(0, BookmarkItem::BOOKMARK_THUMBNAIL_HEIGHT/2)));
    if (index == -1) {
        addItem(bm_item);
    } else {
        insertItem(index, bm_item);
    }
    event->acceptProposedAction();
}

/**
 * @brief BookmarkList::container_drop
 * Creates the new container item and inserts it in the correct index in the bookmark list
 * This new container is an exact copy of the original object
 * @param source : The source list where the container awas located before. Used to get current item
 * @param event
 */
void BookmarkList::container_drop(BookmarkList *source, QDropEvent *event) {
    // BookmarkCategory
    auto item = source->currentItem();
    if (!m_accept_container) {
        event->ignore();
        return;
    }
    auto cast_item = dynamic_cast<BookmarkCategory*>(item);

    BookmarkCategory* cat_item = cast_item->copy();
    int index = row(itemAt(event->pos()+QPoint(0, BookmarkItem::BOOKMARK_THUMBNAIL_HEIGHT/2)));
    if (index == -1) {
        addItem(cat_item);
    } else {
        insertItem(index, cat_item);
    }
    setItemWidget(cat_item, cat_item->get_folder());
    connect(cat_item, &BookmarkCategory::set_bookmark_video, this, &BookmarkList::set_bookmark_video);
    event->acceptProposedAction();
}

/**
 * @brief BookmarkList::rename_item
 * Renames the clicked item
 */
void BookmarkList::rename_item(){
    bool ok;
    switch (clicked_item->type()) {
    case BOOKMARK: {
        auto item = dynamic_cast<BookmarkItem*>(clicked_item);
        BookmarkDialog dialog;

        dialog.setTextValue(QString::fromStdString(item->get_bookmark()->get_description()));
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
        auto item = dynamic_cast<BookmarkCategory*>(clicked_item);
        QString text = QInputDialog::getText(nullptr, "Change title", "Enter a new title", QLineEdit::Normal, QString::fromStdString(item->get_name()), &ok);
        item->update_title(text);
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
    if (currentItem()->type() == BOOKMARK) {
        BookmarkItem* bm_item = dynamic_cast<BookmarkItem*>(currentItem());
        Bookmark* b_mark = bm_item->get_bookmark();
        b_mark->get_video_project()->remove_bookmark(b_mark);
        delete bm_item;
    } else if (currentItem()->type() == CONTAINER) {
        BookmarkCategory* cat_item = dynamic_cast<BookmarkCategory*>(currentItem());
        // Remove all bookmarks from the disputed list
        for (auto bm_item : cat_item->get_disputed()) {
            Bookmark* b_mark = bm_item->get_bookmark();
            b_mark->get_video_project()->remove_bookmark(b_mark);
        }
        // Remove all bookmarks from the references list
        for (auto bm_item : cat_item->get_references()) {
            Bookmark* b_mark = bm_item->get_bookmark();
            b_mark->get_video_project()->remove_bookmark(b_mark);
        }
        delete currentItem();
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
    } else if (m_container_type != UNSORTED && event->button() == Qt::RightButton) {
//        QMenu* menu = new QMenu;
//        menu->addAction("Delete", this, SLOT(remove_item()));
//        menu->exec(mapToGlobal(event->pos()));
//        delete menu;
    } else if (event->button() == Qt::RightButton) {
        QMenu* menu = new QMenu;
        menu->addAction("New category", this, SLOT(add_new_folder()));
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
    auto b_item = dynamic_cast<BookmarkItem*>(item);

    // Start video at correct frame
    VideoState state;
    state = b_item->get_bookmark()->get_state();
    emit set_bookmark_video(b_item->get_bookmark()->get_video_project(), state);
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
    Qt::DropAction drop_action;
    if (event->modifiers() == Qt::ControlModifier) {
        drop_action = drag->exec(Qt::CopyAction);
    } else {
        drop_action = drag->exec(Qt::MoveAction);
    }

    if (drop_action == Qt::MoveAction) {
        auto item = takeItem(row(clicked_item));
        delete item;
    }
    clicked_item = nullptr;
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
 * TODO split into functions
 */
void BookmarkList::dropEvent(QDropEvent *event) {
    // Get subclass type from mimedata
    const QMimeData* mime_data = event->mimeData();
    QByteArray encoded = mime_data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    int type;
    stream >> type;

    // Get origin widget of drag item
    BookmarkList* list = dynamic_cast<BookmarkList*>(event->source());
    if (type == BOOKMARK) {
        bookmark_drop(list, event);
    } else if (type == CONTAINER) {
        container_drop(list, event);
    }
}
