#include "videoeditlist.h"

#include "editdialog.h"
#include "videoedititem.h"

#include <QMenu>
#include <QMessageBox>
#include <QDebug>

VideoEditList::VideoEditList()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setFlow(QListView::TopToBottom);
    horizontalLayout = false;
    setIconSize(QSize(50,50));

    connect(this, &VideoEditList::customContextMenuRequested, this, &VideoEditList::context_menu);
}

void VideoEditList::add_interval(QString path, int start, int end, int last_frame) {
    VideoEditItem* ve_item = new VideoEditItem(path, start, end, last_frame, this);
    qDebug() << count();
    addItem(ve_item);
    qDebug() << count();
    repaint();
}

void VideoEditList::context_menu(const QPoint &point) {
    if(itemAt(point)) {     // R-clicked on item
        clicked_item = itemAt(point);
        setCurrentItem(clicked_item);
        item_right_clicked(point);
    }
    else{                   // R-clicked not on item
        setCurrentItem(nullptr);
        QMenu* menu = new QMenu;
        menu->addAction("Toggle horizontal/vertical view", this, &VideoEditList::toggle_viewlayout);
        menu->addSeparator();
        menu->addAction("Show video", this, &VideoEditList::show_video);
        menu->addAction("Generate Video", this, &VideoEditList::generate_video);
        menu->exec(mapToGlobal(point));
        delete menu;
    }
}


/**
 * @brief VideoEditList::item_right_clicked
 * Will display the context menu when a videoclip or the list is clicked
 * @param pos
 */
void VideoEditList::item_right_clicked(const QPoint pos) {
    QMenu* menu = new QMenu;
    menu->addAction("Remove", this, &VideoEditList::remove_item);
    menu->addAction("Edit interval", this, [this, pos] { edit_item(itemAt(pos)); });
    menu->addSeparator();
    menu->addAction("Toggle horizontal/vertical view", this, &VideoEditList::toggle_viewlayout);
    menu->addAction("Show video", this, &VideoEditList::show_video);
    menu->addAction("Generate Video", this, &VideoEditList::generate_video);

    menu->exec(mapToGlobal(pos));
    delete menu;
}

void VideoEditList::edit_item(QListWidgetItem* item) {
    VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item);
    int start = ve_item->get_start();
    int end = ve_item->get_end();
    EditDialog dialog(&start, &end, ve_item->get_last_frame());
    if(dialog.exec()) {
        ve_item->set_start(start);
        ve_item->set_end(end);
    }
}

/**
 * @brief VideoEditList::remove_item
 * Removes the clicked list item
 */
void VideoEditList::remove_item() {
    QMessageBox msgBox;
    msgBox.setText("remove_item");
    msgBox.exec();
}


/**
 * @brief VideoEditList::toggle_viewlayout
 * Toggles view beween horizontal and vertical
 */
void VideoEditList::toggle_viewlayout() {
    if(horizontalLayout) {
        setFlow(QListView::TopToBottom);
        setIconSize(QSize(50,50));
    } else {
        setFlow(QListView::LeftToRight);
        setIconSize(QSize(110,110));
    }

    horizontalLayout = !horizontalLayout;
}


/**
 * @brief VideoEditList::show_video
 * Shows the video from the videoclip-items in list
 */
void VideoEditList::show_video() {
    QString str;

    for(int i = 0; i < selectedItems().count(); ++i)
    {
        // QListWidgetItem* item = item(i);
        QString tmp = selectedItems().at(i)->text();
        str = str + "\n" + tmp;
    }
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

/**
 * @brief VideoEditList::generate_video
 * Generates video from the videoclip-items in list
 */
void VideoEditList::generate_video() {
    QString str;

    for(int i = 0; i < count(); ++i)
    {
        // QListWidgetItem* item = item(i);
        QString tmp = item(i)->text();
        str = str + "\n" + tmp;
    }
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();

}
