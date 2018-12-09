#include "videoeditlist.h"
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

VideoEditList::VideoEditList()
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    for(int i = 1; i < 10; ++i)
        addItem(new QListWidgetItem(QString("Item %1").arg(i)));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setFlow(QListView::TopToBottom);
    horizontalLayout = false;

    connect(this, &VideoEditList::customContextMenuRequested, this, &VideoEditList::context_menu);

}

void VideoEditList::context_menu(const QPoint &point)
{
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
    qDebug() << "item_right_clicked";

    QMenu* menu = new QMenu;
    menu->addAction("Remove", this, &VideoEditList::remove_item);
    menu->addAction("Toggle horizontal/vertical view", this, &VideoEditList::toggle_viewlayout);
    menu->addSeparator();
    menu->addAction("Show video", this, &VideoEditList::show_video);
    menu->addAction("Generate Video", this, &VideoEditList::generate_video);

    menu->exec(mapToGlobal(pos));
    delete menu;
}



/**
 * @brief VideoEditList::remove_item
 * Removes the clicked list item
 */
void VideoEditList::remove_item()
{
    QMessageBox msgBox;
    msgBox.setText("remove_item");
    msgBox.exec();
}


/**
 * @brief VideoEditList::toggle_viewlayout
 * Toggles view beween horizontal and vertical
 */
void VideoEditList::toggle_viewlayout()
{
    if(horizontalLayout)
        setFlow(QListView::TopToBottom);
    else
        setFlow(QListView::LeftToRight);

    horizontalLayout = !horizontalLayout;

}


/**
 * @brief VideoEditList::show_video
 * Shows the video from the videoclip-items in list
 */
void VideoEditList::show_video()
{
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
void VideoEditList::generate_video()
{
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
