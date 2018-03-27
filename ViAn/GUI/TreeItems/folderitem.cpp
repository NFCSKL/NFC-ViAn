#include "folderitem.h"
#include <QDebug>

FolderItem::FolderItem() : TreeItem(FOLDER_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    const QIcon folder_icon("../ViAn/Icons/folder.png");
    setIcon(0, folder_icon);
}

FolderItem::~FolderItem() {
    qDebug() << "Folder item delete";
}

void FolderItem::remove() {}

void FolderItem::rename(){}
