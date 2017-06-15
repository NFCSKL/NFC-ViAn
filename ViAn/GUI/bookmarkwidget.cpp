#include "bookmarkwidget.h"

BookmarkWidget::BookmarkWidget(QWidget *parent) : QWidget(parent){
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* generate_btn = new QPushButton(tr("Generate"));
    bookmark_list = new QListWidget();

    layout->addWidget(bookmark_list);
    layout->addWidget(generate_btn);
    setLayout(layout);
}

void BookmarkWidget::add_bookmark(std::string description, std::vector<std::string> tags) {
    bookmark_list->addItem(QString::fromStdString(description));
}
