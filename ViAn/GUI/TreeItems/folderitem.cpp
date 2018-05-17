#include "folderitem.h"

FolderItem::FolderItem() : TreeItem(FOLDER_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    const QIcon folder_icon("../ViAn/Icons/folder.png");
    setIcon(0, folder_icon);
}

FolderItem::~FolderItem() {}

void FolderItem::remove() {}

void FolderItem::rename(){}
