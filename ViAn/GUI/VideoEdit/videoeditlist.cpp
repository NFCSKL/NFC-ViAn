#include "videoeditlist.h"
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

VideoEditList::VideoEditList()
{
    setContextMenuPolicy(Qt::CustomContextMenu);

//    for(int i = 1; i < 10; ++i) {
//        //QString iconpath = QString("E:\\screen\\%1.tif").arg(i-1);
//        QString iconpath = "E:\\1.tif";
//        QListWidgetItem *item = new QListWidgetItem(QIcon(iconpath),QString("Item %1").arg(i));
//        //item->setSizeHint(QSize(100,100));
//        addItem(item);

//    }
    QString iconpath = "E:\\still1.jpg";
    QListWidgetItem *item = new QListWidgetItem(QIcon(iconpath),QString("Item %1").arg(1));
    //item->setSizeHint(QSize(100,100));
    addItem(item);
    iconpath = "E:\\still2.png";
    item = new QListWidgetItem(QIcon(iconpath),QString("Item %1").arg(2));
    //item->setSizeHint(QSize(100,100));
    addItem(item);
    iconpath = "E:\\still3.png";
    item = new QListWidgetItem(QIcon(iconpath),QString("Item %1").arg(3));
    //item->setSizeHint(QSize(100,100));
    addItem(item);
    iconpath = "E:\\still4.jpg";
    item = new QListWidgetItem(QIcon(iconpath),QString("Item %1").arg(4));
    //item->setSizeHint(QSize(100,100));
    addItem(item);




    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setFlow(QListView::TopToBottom);
    horizontalLayout = false;
    setIconSize(QSize(50,50));

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
