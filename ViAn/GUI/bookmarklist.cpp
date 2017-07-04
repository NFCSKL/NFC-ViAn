#include "bookmarkwidget.h"
#include "bookmarklist.h"
#include "imagegenerator.h"
#include <QAction>
#include <QMenu>
#include <QMimeData>
#include <QDrag>

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
 * Stores the updated parent container name
 * @param name
 */
void BookmarkList::on_parent_name_edited(QString name) {
    m_par_cont_name = name.toStdString();
    if (m_par_cont_name.empty()) return;
    for (size_t i = 0; i < this->count(); ++i) {
        auto item = this->item(i);
        if (item->type() == 0) {
            // BookmarkItem. Update container name
            auto bm_item = dynamic_cast<BookmarkItem*>(item);
            bm_item->get_bookmark()->set_container_name(m_par_cont_name);
        }
    }
}

void BookmarkList::item_left_clicked() {
}

void BookmarkList::item_right_clicked(const QPoint pos) {
    QMenu menu;
    menu.addAction("Rename", this, SLOT(rename_item()));
    menu.addAction("Remove", this, SLOT(remove_item()));
    menu.exec(mapToGlobal(pos));
}

/**
 * @brief BookmarkList::rename_item
 * Renames the clicked item
 */
void BookmarkList::rename_item(){
    switch (clicked_item->type()) {
    case 0:
        // Bookmark
        break;
    case 1:
        // Container
        break;
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
    switch (event->button()) {
        case Qt::RightButton:
            // Create context menu
            item_right_clicked(event->pos());
            break;
        case Qt::LeftButton:
            // Drag and drop event
            item_left_clicked();
            drag_start_pos = event->pos();
            setCurrentItem(clicked_item);
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
    std::cout << this->count() << std::endl;
    if (event->modifiers() == Qt::ControlModifier) {
        drop_action = drag->exec(Qt::CopyAction);
    } else {
        drop_action = drag->exec(Qt::MoveAction);
    }


    if (drop_action == Qt::MoveAction) {
        auto item = takeItem(row(clicked_item));
        delete item;
        std::cout << this->count() << std::endl;
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
 */
void BookmarkList::dropEvent(QDropEvent *event) {
    // Get subclass type from mimedata
    const QMimeData* mime_data = event->mimeData();
    QByteArray encoded = mime_data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    int type;
    stream >> type;

    // Get origin widget of drag item
    QListWidget* list = dynamic_cast<BookmarkList*>(event->source());
    auto item = list->currentItem();
    if (type == 0) {
        // BookmarkItem. Copy and add
        auto cast_item = dynamic_cast<BookmarkItem*>(item);
        QIcon icon = cast_item->icon();
        BookmarkItem* bm_item = new BookmarkItem(cast_item->get_bookmark(), 0);
        bm_item->get_bookmark()->set_container_name(m_par_cont_name);
        bm_item->get_bookmark()->set_type(m_container_type);
        bm_item->setIcon(icon);
        addItem(bm_item);
        event->acceptProposedAction();
    } else if (type == 1 ) {
        // BookmarkCategory
        if (!m_accept_container) {
            event->ignore();
            return;
        }

        auto cast_item = dynamic_cast<BookmarkCategory*>(item);
        BookmarkCategory* bm_cat = new BookmarkCategory(cast_item->get_name(), this, 1);

        for (auto item : cast_item->get_disputed()) {
            if (item == nullptr) {
                qDebug() << "SOMETHING WENT WRONG";
                continue;
            }
            BookmarkItem* b_item = new BookmarkItem(item->get_bookmark(),0);
            b_item->setIcon(item->icon());
            bm_cat->add_disp_item(b_item);
        }

        for (BookmarkItem* item : cast_item->get_references()) {
            if (item == nullptr) {
                qDebug() << "SOMETHING WENT WRONG";
                continue;
            }
            BookmarkItem* b_item = new BookmarkItem(item->get_bookmark(),0);
            b_item->setIcon(item->icon());
            bm_cat->add_ref_item(b_item);
        }

        addItem(bm_cat);
        event->acceptProposedAction();
    }
}
