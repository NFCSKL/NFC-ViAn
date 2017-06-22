#include "bookmarkwidget.h"
#include "Library/customdialog.h"
BookmarkWidget::BookmarkWidget(QWidget *parent) : QWidget(parent){

    QPushButton* generate_btn = new QPushButton(tr("Generate"));
    QPushButton* new_folder_btn = new QPushButton(tr("New folder"));
    connect(new_folder_btn, &QPushButton::clicked, this, &BookmarkWidget::add_new_folder);
    bm_win = get_drag_drop_list(this);

    bm_win_layout = new QVBoxLayout();
    bm_win_layout->setAlignment(Qt::AlignTop);
    bm_win_layout->setSpacing(10);
    bm_win_layout->setMargin(10);
    scroll_area = new QScrollArea();

    bm_win->setLayout(bm_win_layout);
    scroll_area->setWidget(bm_win);
    scroll_area->setFrameStyle(0);
    scroll_area->setWidgetResizable(true);

    layout = new QVBoxLayout();    
    layout->setAlignment(Qt::AlignBottom);    
    layout->addWidget(new_folder_btn);
    layout->addWidget(scroll_area);
    layout->addWidget(generate_btn);   
    layout->setMargin(10);
    layout->setSpacing(10);
    setMinimumWidth(bm_win->sizeHint().width()*2); // Should be 2*thumbnail + margin
    setLayout(layout);
}

void BookmarkWidget::add_new_folder()
{
    BookmarkCategory* f2 = new BookmarkCategory(bm_win);
    bm_win->addItem(f2);

}

void BookmarkWidget::create_bookmark(VideoProject* vid_proj, const int frame_nbr, cv::Mat)
{
    qDebug() << "create bookmark";
    bool ok;
    QString text = get_input_text("", &ok);    
    if(!ok) return;
    qDebug() << "add bookmark";
    Bookmark* bookmark = new Bookmark(vid_proj,text.toStdString(), frame_nbr);
    vid_proj->add_bookmark(bookmark);
    //add_bookmark(vid_proj, bookmark);
}

QListWidget* BookmarkWidget::get_drag_drop_list(QWidget* parent)
{
    QListWidget* lw = new QListWidget(parent);
    // Enable drag and drop
    lw->setSelectionMode(QAbstractItemView::SingleSelection);
    lw->setDragDropMode(QAbstractItemView::InternalMove);
    lw->setDragEnabled(true);
    lw->viewport()->setAcceptDrops(true);
    lw->setDropIndicatorShown(true);
    return lw;
}
/**
 * @brief BookmarkWidget::get_input_text
 * @param bookmark_text Text shown in the text edit when opening the dialog.
 * @param ok Parameter set to false if the user cancels.
 * @return Returns a description for the bookmark,
 *         obtained from the user.
 */
QString BookmarkWidget::get_input_text(std::string text, bool* ok) {
    // Create the dialog
    CustomDialog dialog("Bookmark description", NULL);
    dialog.addLabel("Write a description of the bookmark:");
    dialog.addTextEdit(&text, false, false, TEXT_EDIT_MIN_HEIGHT,
                          "Write a description of the bookmark. This will be used when creating a report.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        *ok = false;
        return "";
    }
    *ok = true;
    return QString::fromStdString(text);
}
