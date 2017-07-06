#include "bookmarkwidget.h"
#include "bookmarklist.h"
#include "imagegenerator.h"
#include <QAction>
#include <QMenu>
#include <QMimeData>
#include <QDrag>
#include <algorithm>

// remove
#include <iostream>
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
    connect(this, &BookmarkList::itemDoubleClicked, this, &BookmarkList::on_double_clicked);
    clear();
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
void BookmarkList::set_parent_name(string name) {
    m_par_cont_name = name;
}

/**
 * @brief BookmarkList::on_parent_name_edited
 * Stores the updated parent container name in each bookmark
 * @param name
 */
void BookmarkList::on_parent_name_edited(QString name) {
    if (m_par_cont_name.empty()) return;
    for (size_t i = 0; i < this->count(); ++i) {
        auto item = this->item(i);
        if (item->type() == 0) {
            // BookmarkItem. Update container name
            auto bm_item = dynamic_cast<BookmarkItem*>(item);
            bm_item->get_bookmark()->rename_container(m_par_cont_name, name.toStdString());
        }
    }
    m_par_cont_name = name.toStdString();
}

void BookmarkList::item_left_clicked() {
}

void BookmarkList::item_right_clicked(const QPoint pos) {
    QMenu* menu = new QMenu;
    QString rename = (clicked_item->type() == 0) ? "Change description" : "Change title";
    menu->addAction(rename, this, SLOT(rename_item()));
    menu->addAction("Delete", this, SLOT(remove_item()));
    menu->exec(mapToGlobal(pos));
    delete menu;
}

void BookmarkList::bookmark_drop(BookmarkList *source, QDropEvent *event) {
    // BookmarkItem. Copy and add
    auto item = source->currentItem();
    auto cast_item = dynamic_cast<BookmarkItem*>(item);
    BookmarkItem* bm_item = cast_item->copy();
    bm_item->get_bookmark()->add_container(m_par_cont_name, m_container_type);
    addItem(bm_item);
    if (event->proposedAction() == Qt::MoveAction) {
        // Remove the bookmark from the old container
        bm_item->get_bookmark()->remove_container(source->m_par_cont_name, source->m_container_type);
    }
    event->acceptProposedAction();
}

void BookmarkList::container_drop(BookmarkList *source, QDropEvent *event) {
    // BookmarkCategory
    auto item = source->currentItem();
    if (!m_accept_container) {
        event->ignore();
        return;
    }

    auto cast_item = dynamic_cast<BookmarkCategory*>(item);
    addItem(cast_item->copy(this));
    event->acceptProposedAction();
}


/**
 * @brief BookmarkList::rename_item
 * Renames the clicked item
 */
void BookmarkList::rename_item(){
    bool ok;
    switch (clicked_item->type()) {
    case 0: {
        // Bookmark
        auto item = dynamic_cast<BookmarkItem*>(clicked_item);
        QString text = QInputDialog::getMultiLineText(nullptr, "Change description",
                                                      "Enter a new discription", QString::fromStdString(item->get_bookmark()->get_description()), &ok);
        item->update_description(text);
        break;
    }
    case 1:{
        // Container
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
    delete currentItem();
}

void BookmarkList::on_double_clicked(QListWidgetItem *item) {
    if (item->type() != 0) return;
    auto b_item = dynamic_cast<BookmarkItem*>(item);
    // Start video at correct frame

    emit set_bookmark_video(b_item->get_bookmark()->get_video_project(), b_item->get_frame_number());
}

/**
 * @brief BookmarkList::mousePressEvent
 * Triggers when the widget is clicked.
 * Will create context menu on right click and
 * setup some value for drop on left click.
 * @param event
 */
void BookmarkList::mousePressEvent(QMouseEvent *event) {
    if (!itemAt(event->pos())) return;
    clicked_item = itemAt(event->pos());
    on_double_clicked(clicked_item);
    setCurrentItem(clicked_item);
    switch (event->button()) {
        case Qt::RightButton:
            // Create context menu
            item_right_clicked(event->pos());
            break;
        case Qt::LeftButton:
            // Drag and drop event
            item_left_clicked();
            drag_start_pos = event->pos();
            break;
        default:
        break;
    }
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
}

void BookmarkList::dragEnterEvent(QDragEnterEvent *event) {
    // Only accpt the event if the drop item is of the correct type
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        if (event->keyboardModifiers() == Qt::ControlModifier) {
            event->setDropAction(Qt::CopyAction);
        } else {
            event->setDropAction(Qt::MoveAction);
        }
        event->accept();
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
    auto item = list->currentItem();
    if (type == 0) {
        bookmark_drop(list, event);
    } else if (type == 1 ) {
        container_drop(list, event);
    }
}
