#include "reportgenerator.h"

#include "GUI/Bookmark/bookmarkitem.h"
#include "GUI/Bookmark/bookmarkcategory.h"
#include "Project/bookmark.h"
#include "utility.h"

#include <ActiveQt/QAxObject>
#include <QDebug>
#include <QDir>
#include <QTextStream>

#include <ctime>

/**
 * @brief ReportGenerator::ReportGenerator
 * The constructor will assign parameters to the class members.
 * @param proj, the current project that we are creating document for.
 * @param file_handler, the file_handler that is used to get path information for saving.
 */
ReportGenerator::ReportGenerator(QString proj_path, std::vector<QListWidgetItem*> items) {
    m_path = proj_path;
    list_items = items;
    word = new QAxObject("Word.Application");
}

/**
 * @brief ReportGenerator::~ReportGenerator
 * destructor to remove allocated memory.
 */
ReportGenerator::~ReportGenerator() {
    delete word;
}

/**
 * @brief ReportGenerator::create_report
 * This method will do 5 steps to create a word document, load it with bookmarks and save it.
 */
void ReportGenerator::create_report() {
    //1. START APPLICATION
    if(!word->isNull()){
        //2. CREATE AND OPEN THE DOCUMENT;
        QAxObject* doc = word->querySubObject("Documents");
        doc->dynamicCall("Add()");
        QAxObject* active_doc = word->querySubObject("ActiveDocument");
        //4. ADD IMAGES FROM BOOKMARK FOLDER
        create_bookmark_table(active_doc);
        //5. SAVE AND CLOSE FILE
        QString path = save_report(active_doc);
        path.replace("/", "\\\\");
        close_report(doc);
        doc->dynamicCall("Open(QString path)", path);
    } else {
        qWarning("Could not find Word instance");
    }
}

/**
 * @brief ReportGenerator::get_bookmark_descr
 * Create a string from the description of the bookmark bm.
 * The string contains all the information that will be printed in the report.
 * @param bm
 * @return
 */
QString ReportGenerator::get_bookmark_descr(BookmarkItem *bm) {
    QString img_file = bm->get_bookmark()->m_image_name;
    QString time = QString("Time: %1").arg(bm->get_time());
    QString brightness = QString("Brightness: %1").arg(bm->get_bookmark()->get_state().brightness);
    QString contrast = QString("Contrast: %1").arg(bm->get_bookmark()->get_state().contrast);
    QString gamma = QString("Gamma: %1").arg(bm->get_bookmark()->get_state().gamma);
    QString description = bm->get_description();
    QString bm_description = img_file + QString("\v") + time + QString("\vCorrection: ") + brightness + " " + contrast + " " + gamma;
    if (description != "") {
        bm_description = description + QString("\v") + bm_description;
    }
    return QString("\v") + bm_description;
}

/**
 * @brief ReportGenerator::resize_picture
 * This method will make all images to be of the same size. All images will keep
 * its aspect ratio.
 * @param pic_path, path to the bookmark that is to be resized.
 * @param inline_shape, A word specific object that is a shape where its
 * layout is "inline" with the rest of the document.
 *
 * TODO Unused
 */
void ReportGenerator::resize_picture(QString pic_path, QAxObject* inline_shape) {
    QImage image = QImage(pic_path);
    int const original_height = image.height();
    int const original_width = image.width();
    double img_width = inline_shape->property("Width").toDouble();
    //Scale all images to have same width but keep aspect ratio
    double multiply_factor = img_width/original_width;
    inline_shape->dynamicCall( "Height", (multiply_factor*original_height));
    inline_shape->dynamicCall( "Width", img_width);
}

/**
 * @brief ReportGenerator::create_bookmark_table
 * This method will add all bookmarks for the current project
 * to the document.
 * @param para, document to add the bookmarks to
 */
void ReportGenerator::create_bookmark_table(QAxObject* para) {
    // The end of the previous range
    int end = 0;
    for (auto item : list_items) {
        if (item->type() == 1) {            // Type = CONTAINER
            end = add_category(para, end, item);
        } else if (item->type() == 0) {     // Type = BOOKMARK
            end = add_bookmark(para, end, item);
        }
    }
}

int ReportGenerator::add_category(QAxObject* para, int end, QListWidgetItem* item) {
    BookmarkCategory* cat = dynamic_cast<BookmarkCategory*>(item);
    // 3 rows for the title, category name and the category
    auto table_rows = 3;
    // Space to use for table
    QAxObject* range = para->querySubObject("Range()");
    range->dynamicCall("SetRange(int,int)", end, end+2000);

    QAxObject* tables = range->querySubObject("Tables");
    QAxObject* table = tables->querySubObject("Add(QVariant,int,int)",range->asVariant(), table_rows,2,1,1);
    table->dynamicCall("AutoFormat(QVariant)", BORDER);
    table->dynamicCall("SetTitle(QString)", "Title");
    range->dynamicCall("Collapse(int)", 0);
    // Add the category to the space
    // Table indexed from 1, begin after title.
    int cell_row = 1;

    // Access duplicate category title cells and merge them together
    QAxObject* _tmp_title = table->querySubObject("Cell(int,int)", cell_row, 1);
    QAxObject* _tmp_title2 = table->querySubObject("Cell(int,int)", cell_row, 2);
    _tmp_title->dynamicCall("Merge(IDispatch*)", _tmp_title2->asVariant());
    // Write category name
    cell_add_text(table, cat->get_name(), cell_row, 1);

    // Go to next table row
    cell_row++;

    // Add title text
    cell_add_text(table, "Omstritt", cell_row, 1);
    cell_add_text(table, "Referens", cell_row, 2);

    cell_row++;

    // Access disputed and reference bookmarks
    std::vector<BookmarkItem*> bm_disp = cat->get_disputed();
    std::vector<BookmarkItem*> bm_ref = cat->get_references();

    // Access cells to be used for storing disp and ref bookmarks
    QAxObject* cell_disp = table->querySubObject("Cell(int,int)", cell_row, 1);
    QAxObject* cell_ref = table->querySubObject("Cell(int,int)", cell_row, 2);

    // Insert categories into disputed and reference cells.
    cell_insert_category(cell_disp, bm_disp);
    cell_insert_category(cell_ref, bm_ref);

    // Insert empty line and update the end position
    range->dynamicCall("InsertParagraphAfter()");
    return range->dynamicCall("End()").toInt();
}

int ReportGenerator::add_bookmark(QAxObject* para, int end, QListWidgetItem* item) {
    BookmarkItem* bm = dynamic_cast<BookmarkItem*>(item);
    // Access Bookmark
    QString file_name = bm->get_file_path();
    file_name.replace("/", "\\\\");

    QAxObject* bmark_range = para->querySubObject("Range()");
    bmark_range->dynamicCall("SetRange(int,int)", end, end+100);
    // Get the last position of that range

    // Insert the bookmark description, divider line and bookmark image as a stack
    // because they are all inserted at the same position
    bmark_range->dynamicCall("InsertAfter(QString Text)", get_bookmark_descr(bm));
    bmark_range->dynamicCall("InsertParagraphAfter()");
    QAxObject* p_shapes = para->querySubObject("InlineShapes()");
    p_shapes->dynamicCall("AddPicture(const QString&,bool,bool,QVariant)",
                 file_name, false, true, bmark_range->asVariant());
    return bmark_range->dynamicCall("End()").toInt();
}

/**
 * @brief ReportGenerator::cell_insert_category
 * @param cell
 * @param bm_list
 * Inserts a nested table into range, container a category
 */
void ReportGenerator::cell_insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list) {
    // Access field to use for category table
    QAxObject* cell_range = cell->querySubObject("Range");
    // Table indexed from 1, no title so start at 1.
    int cell_row = 1;
    // Number of rows required for Category list
    int max_row = bm_list.size();
    // If empty category, no bookmarks to add => return
    if(max_row == 0) return;
    // Create table for category list, requires max_row > 0
    QAxObject* table = add_table(cell_range, max_row, 1);

    // For every bookmark
    for(size_t j = 0; j != bm_list.size(); ++j){
        // Access Bookmark
        BookmarkItem* bm = bm_list.at(j);
        // Add Its image to current cell
        cell_add_img(table, bm->get_file_path(), cell_row, 1);
        // Add Its text to current cell
        cell_add_text(table, get_bookmark_descr(bm), cell_row, 1);
        // Go to next cell
        cell_row++;
    }
}
/**
 * @brief ReportGenerator::cell_add_img
 * @param table
 * @param file_name
 * @param row
 * @param col
 * Adds image to cell
 */
void ReportGenerator::cell_add_img(QAxObject *table, QString file_name, int row, int col) {
    // Access cell to add image to
    QAxObject* cell = table->querySubObject("Cell(int,int)",row,col);
    // Access area to add image to
    QAxObject* range = cell->querySubObject("Range");
    // Access list of shapes
    QAxObject* shapes = range->querySubObject("InlineShapes");
    //Fix to make path work with windows word
    //application when spaces are involved
    file_name.replace("/", "\\\\");
    // Add image and scale it to fit cell
    shapes->querySubObject(
                "AddPicture(const QString&,bool,bool,QVariant)",
                 file_name, false, true, range->asVariant());
}

/**
 * @brief ReportGenerator::cell_add_text
 * @param table
 * @param entry
 * @param row
 * @param col
 * Appends text to range
 */
void ReportGenerator::cell_add_text(QAxObject* table, QString entry, int row, int col) {
    // Access area to add text to
    QAxObject* range = table->querySubObject("Cell(int,int)", row, col)->querySubObject("Range");
    // Add it to end of area
    range->dynamicCall("InsertAfter(QString Text)", entry);
}

/**
 * @brief ReportGenerator::make_doc
 * @param obj
 * @param file_name
 *
 * Creates documentation for a given QAxObject*,
 * this function is extremely useful for using word object API.
 */
void ReportGenerator::make_doc(QAxObject *obj, QString file_name) {
    QDir dir;
    dir.mkpath("C:/Documents/WORD_COM/");
    QFile file1("C:/Documents/WORD_COM/"+ file_name + ".html");
       file1.open(QIODevice::WriteOnly | QIODevice::Text);
       QTextStream out(&file1);
       out << obj->generateDocumentation();
       file1.close();
}

/**
 * @brief ReportGenerator::add_table
 * @param range
 * @param rows
 * @param cols
 * @param style
 * @return Table*
 * Add table to given range with given style
 */
QAxObject* ReportGenerator::add_table(QAxObject *range, int rows, int cols, TABLE_STYLE style) {
    range->dynamicCall("Collapse(int)",1); // Don't touch this, magically works
    QAxObject* tables = range->querySubObject("Tables");
    QAxObject* table = tables->querySubObject("Add(QVariant,int,int)",range->asVariant(), rows,cols,1,1);
    table->dynamicCall("AutoFormat(QVariant)", QVariant(style));
    table->dynamicCall("SetTitle(QString)", "Title");
    return table;
}

/**
 * @brief ReportGenerator::date_time_generator
 * calculates the current time and date and changes the format
 * to use "_" instead of ":" since it is not allowed in word application
 * to save file names that have a colon in the name.
 * @return string, the current time and date.
 */
QString ReportGenerator::date_time_generator() {
    time_t now = time(nullptr);
    QString dt = ctime(&now);
    dt.replace(':', '-');
    dt.replace(' ' , '_');
    dt.remove('\n');
    //dt.erase(std::remove(dt.begin(), dt.end(), '\n'), dt.end());
    return dt;
}

/**
 * @brief ReportGenerator::save_report
 * This method will save the word document in the project folder.
 * The name will be based on the project and current date and time.
 * @param active_document, the document that is selected
 */
QString ReportGenerator::save_report(QAxObject* active_document) {
    QString dt = date_time_generator();
    QString proj_name = m_path.left(m_path.lastIndexOf("/"));   // Strip away last "/"
    proj_name = Utility::name_from_path(proj_name);
    QString path = m_path.append(proj_name).append("_").append(dt).append(".docx");
    active_document->dynamicCall("SaveAs (const QString&)", path);
    return path;
}

/**
 * @brief ReportGenerator::close_report
 * This method will close the document that are sent in
 * as a parameter.
 * @param doc, document instance
 */
void ReportGenerator::close_report(QAxObject* doc) {
    doc->dynamicCall("Close()");
}
