#include "bookmarkcategory.h"

BookmarkCategory::BookmarkCategory(QListWidget *parent) : QListWidgetItem(parent)
{   
    // Setup layout
    QWidget* folder = new QWidget();
    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setMargin(5);
    layout->setSpacing(5);
    folder->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QHBoxLayout* container = new QHBoxLayout();
    QLineEdit* title = new QLineEdit("mapname");
    layout->addWidget(title);
    layout->addLayout(container);

    // Add disputed and reference list
    disputed = make_scrollable_container();
    reference = make_scrollable_container();

    container->addWidget(disputed);
    container->addWidget(reference);

    folder->setLayout(layout);
    parent->setItemWidget(this, folder);
    setSizeHint(folder->sizeHint());
}

QScrollArea *BookmarkCategory::make_scrollable_container()
{
    BookmarkContainer* container = new BookmarkContainer();
    QScrollArea* scroll_area = new QScrollArea();
    scroll_area->setWidget(container);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);
    return scroll_area;
}


