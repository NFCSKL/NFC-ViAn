#include "report.h"
#include "utility.h"
/**
 * @brief Report::Report
 * @param file_path
 */
Report::Report(std::string file_path){
    this->file_path = file_path;
}
/**
 * @brief Report::Report
 * Empty constructor, required for read
 */
Report::Report(){
}

/**
 * @brief Report::reset_root_dir
 * @param dir
 */
void Report::reset_root_dir(const std::string &dir)
{
    file_path = dir+Utility::name_from_path(file_path);
}

/**
 * @brief Report::read
 * @param json
 */
void Report::read(const QJsonObject& json){
    this->file_path = json["report_path"].toString().toStdString();
}
/**
 * @brief Report::write
 * @param json
 */
void Report::write(QJsonObject& json){
    json["report_path"] = QString::fromStdString(this->file_path);
}

/**
 * @brief Report::get_file_path
 * @return
 */
std::string Report::get_file_path() const{
    return file_path;
}
