#include "bookmarkview.h"
#include "Library/customdialog.h"
#include <iostream>

/**
 * @brief BookmarkView::BookmarkView
 * Constructor.
 * @param parent Parent item of the BookmarkView.
 */
BookmarkView::BookmarkView(QListWidget* parent) {
    view = parent;
}

/**
 * @brief BookmarkView::get_num_bookmarks
 * @return Returns the number of bookmarks.
 */
int BookmarkView::get_num_bookmarks() {
    return view->count();
}

/**
 * @brief BookmarkView::add_bookmark
 * Adds a bookmark containing an image (thumbnail)
 * and a text description of the bookmark.
 * @param frame_nbr Frame number associated with the bookmark.
 * @param file_path Path to the image of the bookmark.
 */
void BookmarkView::add_bookmark(Bookmark* bookmark) {
    bool ok;
    QString bookmark_text = get_input_text(&ok);
    if (ok) {
        BookmarkItem* bookmark_item = new BookmarkItem(bookmark, view);
        view->addItem(bookmark_item);
    }
}

/**
 * @brief BookmarkView::add_bookmark
 * Adds a bookmark to the bookmark view.
 * @param bookmark Bookmark to add.
 */
void BookmarkView::add_bookmark(Bookmark bookmark) {
//    BookmarkItem* bookmark_item = new BookmarkItem(bookmark, view);
//    view->addItem(bookmark_item);
    std::cout << "lol xDD" << std::endl;
}

/**
 * @brief BookmarkView::get_input_text
 * @param ok Parameter set to false if the user cancels.
 * @return Returns a description for the bookmark,
 *         obtained from the user.
 */
QString BookmarkView::get_input_text(bool* ok) {
    std::string bookmark_text = "";

    // Create the dialog
    CustomDialog dialog("Bookmark description", NULL);
    dialog.addLabel("Write a description of the bookmark:");
    dialog.addTextEdit(&bookmark_text, false, false, TEXT_EDIT_MIN_HEIGHT,
                          "Write a description of the bookmark. This will be used when creating a report.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        *ok = false;
        return "";
    }
    *ok = true;
    return QString::fromStdString(bookmark_text);
}
