#ifndef REPORTGENERERATOR_H
#define REPORTGENERERATOR_H

#include <iostream>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include <QString>
#include "Project/project.h"
#include "dirent.h"
#include "GUI/Bookmark/bookmarkitem.h"
#include <ctime>
#include "utility.h"
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
 * Mainly to methods are used (very few are available)
 *
 * obj->dynamicCall("SomeString(param1, param2)", param1,param2)
 * is used to make function calls

* obj_return = obj->querySubObject("Range") is a request
 * for member Range in obj. querySubObject is however used whenever
 * a returned object is to be used in some way after the call.
 *
 */
class ReportGenerator : public QObject {
    Q_OBJECT
    std::string m_path;
    QAxObject* word;
    std::vector<Bookmark*> all_bookmarks;
    const double IMAGE_WIDTH_REFERENCE = 136.0;
    ReportContainer m_rep_cont;
public:
    friend class test_report_generator;
    explicit ReportGenerator(std::string proj_path,ReportContainer report_container);
    ~ReportGenerator();
    void create_report();    
private:
    /**
     * Creates documentation for a given QAxObject*,
     * this function is extremely useful for using word object API.
     */
    static void make_doc(QAxObject* obj, QString file_name);

    void add_paragraph(QAxObject* selection);

    QString get_bookmark_descr(BookmarkItem *bm);
    void create_bookmark_table(QAxObject *para, ReportContainer bookmark_list);

    QAxObject* add_table(QAxObject* range, int rows, int cols, TABLE_STYLE style= NO_BORDER);
    QAxObject* get_cell(QAxObject* table, int row, int cols);

    void resize_picture(QString pic_path, QAxObject* inline_shape);
    // Methods operating on a cell in a table
    void cell_insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list);
    void cell_add_bookmark();
    void cell_add_text(QAxObject* range, QString entry, int row, int col);
    void cell_add_img(QAxObject* table, QString file_name, int row, int col);

    // Methods Finishing up reportgeneraion
    QString save_report(QAxObject* active_document);
    void close_report(QAxObject* doc, QAxObject*  word);

    // Formatting methods
    std::string date_time_generator();
    QString calculate_time(int ms);
signals:
    void done(void);
};
#endif // REPORTGENERERATOR_H
