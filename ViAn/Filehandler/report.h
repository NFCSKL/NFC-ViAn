#ifndef REPORT_H
#define REPORT_H
#include <string>
#include "saveable.h"
class Report : public Saveable{
public:
    Report(std::string file_path);
    std::string file_path;
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // REPORT_H
