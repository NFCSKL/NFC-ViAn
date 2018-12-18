#ifndef REPORTGENERERATOR_H
#define REPORTGENERERATOR_H

#include <QObject>

class BookmarkItem;
class QAxObject;
class QListWidgetItem;

enum TABLE_STYLE {NO_BORDER = 0, BORDER=36};
using RefDisp = std::pair<std::vector<BookmarkItem*>,std::vector<BookmarkItem*>>;
using Category = std::pair<QString,RefDisp>;
using ReportContainer = std::vector<Category>;

/**
 * @brief The ReportGenerator class
 * This class is used to generate reports from
 * a vector of categories.
 * It uses Words COM interface and QTs ActiveQt tool with
 * QAxObjects to communicate with Word.
 *
 * Mainly two methods are used (very few are available)
 *
 * obj->dynamicCall("SomeString(param1, param2)", param1,param2)
 * is used to make function calls
 *
 * obj_return = obj->querySubObject("Range") is a request
 * for member Range in obj. querySubObject is however used whenever
 * a returned object is to be used in some way after the call.
 */
class ReportGenerator : public QObject {
    Q_OBJECT
    QString m_path;
    QAxObject* word;
public:
    friend class test_report_generator;
    explicit ReportGenerator(QString proj_path, std::vector<QListWidgetItem *> items);
    ~ReportGenerator();
    void create_report();

    std::vector<QListWidgetItem*> list_items;
private:
    /**
     * Creates documentation for a given QAxObject*,
     * this function is extremely useful for using word object API.
     */
    static void make_doc(QAxObject* obj, QString file_name);

    void add_paragraph(QAxObject* selection);

    QString get_bookmark_descr(BookmarkItem *bm);
    void create_bookmark_table(QAxObject *para);

    int add_category(QAxObject *para, int end, QListWidgetItem *item);
    int add_bookmark(QAxObject *para, int end, QListWidgetItem *item);
    QAxObject* add_table(QAxObject* range, int rows, int cols, TABLE_STYLE style = NO_BORDER);
    QAxObject* get_cell(QAxObject* table, int row, int cols);

    void resize_picture(QString pic_path, QAxObject* inline_shape);
    // Methods operating on a cell in a table
    void cell_insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list);
    void cell_add_bookmark();
    void cell_add_text(QAxObject* range, QString entry, int row, int col);
    void cell_add_img(QAxObject* table, QString file_name, int row, int col);

    // Methods finishing up reportgeneration
    QString save_report(QAxObject* active_document);
    void close_report(QAxObject* doc);

    // Formatting methods
    QString date_time_generator();
signals:
    void done(void);
};
#endif // REPORTGENERERATOR_H
