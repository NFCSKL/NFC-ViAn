#ifndef REPORTGENERERATOR_H
#define REPORTGENERERATOR_H

#include <iostream>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include <QString>
#include "Filehandler/filehandler.h"
#include  "dirent.h"
#include <ctime>

class ReportGenerator
{
public:
    ReportGenerator(std::string& bookmark_path);


private:
    QString picPath;
    std::string proj_path;
    std::string bookmark_path;
    std::vector<std::string> bookmark_names;
    void create_list_of_names();
    void add_pictures(QAxObject* selection);
    void save_report(QAxObject* activeDocument);
    void close_report(QAxObject* doc, QAxObject*  word);
    std::string date_time_generator();
};
#endif // REPORTGENERERATOR_H

