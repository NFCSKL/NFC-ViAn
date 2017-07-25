#ifndef REPORT_H
#define REPORT_H
#include <string>
#include "Filehandler/saveable.h"
class Report : public Writeable{
    std::string file_path;
public:
    Report(std::string file_path);
    Report();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    std::string get_file_path() const;
};

#endif // REPORT_H
