#ifndef REPORTGENERERATOR_H
#define REPORTGENERERATOR_H

#include <iostream>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include <QString>
#include "Filehandler/filehandler.h"
#include  "dirent.h"
#include <ctime>

class ReportGenerator {

public:
    friend class test_report_generator;
    explicit ReportGenerator(Project* proj, FileHandler* file_handler);
    ~ReportGenerator();
    void create_report();
private:

    Project* proj;
    QAxObject* word;
    FileHandler *file_handler;
    std::vector<std::pair<std::string, std::string> > all_bookmarks;

    const double IMAGE_WIDTH_REFERENCE = 272.0;

    void create_list_of_names();
    void add_bookmarks(QAxObject* selection);
    QString save_report(QAxObject* active_document);
    void close_report(QAxObject* doc, QAxObject*  word);
    void resize_picture(QString pic_path, QAxObject* inline_shape);
    std::string date_time_generator();
    void add_paragraph(QAxObject* selection);
};
#endif // REPORTGENERERATOR_H

