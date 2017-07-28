#ifndef AXCELL_H
#define AXCELL_H
#include <QAxObject>
#include <GUI/bookmarkitem.h>
#include "axrange.h"
#include "axtable.h"
class AxCell
{
    QAxObject* m_cell;
    AxTable m_table;
    AxRange m_range;
    int m_row;
    int m_col;
public:
    AxCell(int row,int col);

    void insert_category(QAxObject* m_cell, std::vector<BookmarkItem *> bm_list);
    void add_bookmark();
    void add_text(QAxObject* range, QString entry, int _row, int col);
    void add_img(QAxObject* table, QString file_name, int _row, int col);

    AxRange range() const;
    int row() const;
    int col() const;
};

#endif // AXCELL_H
