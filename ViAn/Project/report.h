#ifndef REPORT_H
#define REPORT_H
#include <string>
#include "Filehandler/saveable.h"
class Report : public Writeable{
    std::string file_path;
public:
    Report(std::string file_path);
    Report();
    void reset_root_dir(const std::string& dir);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    std::string get_file_path() const;
};

#endif // REPORT_H
