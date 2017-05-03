#include "report.h"

Report::Report(std::string file_path)
{
    this->file_path = file_path;
}
void Report::read(const QJsonObject& json){
    this->file_path = json["report_path"].toString().toStdString();
}

void Report::write(QJsonObject& json){
    json["report_path"] = QString::fromStdString(this->file_path);
}
