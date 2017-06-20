#include "saveable.h"

const Saveable::SAVE_FORMAT Saveable::DEFAULT_SAVE_FORMAT;


std::string Saveable::full_path() const
{
    return m_full_path;
}

Saveable::Saveable(){

}
/**
 * @brief Saveable::~Saveable
 * virtual destructor
 */
Saveable::~Saveable(){

}

/**
 * @brief FileHandler::save_saveable
 * @param savable
 * @param dir_path
 * @param save_format
 * @return Saves a Json file to provided directory with file_name and save_format
 */
bool Saveable::save_saveable(const std::string& file_name,
                             const std::string& dir_path, const SAVE_FORMAT& save_format){
    QDir dir;
    dir.mkpath(QString::fromStdString(dir_path));
    QFile save_file(save_format == JSON
                    ? QString::fromStdString(dir_path + "/" + file_name +".json")
                    : QString::fromStdString(dir_path + "/" + file_name + ".dat"));
    if(!save_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject json_saveable;
    this->write(json_saveable);
    QJsonDocument save_doc(json_saveable);
    save_file.write(save_format == JSON
            ? save_doc.toJson()
            : save_doc.toBinaryData());
    m_full_path = (file_name.find(".json") != std::string::npos ||
            file_name.find(".dat") != std::string::npos)
            ? dir_path + "/" + file_name
            : save_file.fileName().toStdString();

    return true;
}

/**
 * @brief Saveable::save_saveable
 * @param full_path
 * @param save_format
 * @return true if successfull, false else
 */
bool Saveable::save_saveable(const std::string &file_name, const Saveable::SAVE_FORMAT &save_format)
{
    QFile save_file(save_format == JSON     // Decide what format was used
                    ? QString::fromStdString(file_name + ".json")
                    : QString::fromStdString(file_name + ".dat"));
    if(!save_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject json_saveable;
    this->write(json_saveable);                        //Write to json
    QJsonDocument save_doc(json_saveable);
    save_file.write(save_format == JSON
            ? save_doc.toJson()
            : save_doc.toBinaryData());
    m_full_path = (file_name.find(".json") != std::string::npos ||
            file_name.find(".dat") != std::string::npos)
            ? file_name
            : save_file.fileName().toStdString();
    return true;
}

/**
 * @brief Saveable::delete_saveable
 * @return
 */
bool Saveable::delete_saveable()
{
    QFile file(QString::fromStdString(m_full_path));
    file.remove();
}

/**
 * @brief Saveable::load_savable
 * @param full savable path
 * @param save_format
 * @return loaded Savable
 * Loads saveable from binary or json file.
 */
bool Saveable::load_saveable(const std::string& full_path, const SAVE_FORMAT& save_format){
    QFile load_file(QString::fromStdString(full_path));
    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file %s. ", load_file.fileName().toStdString().c_str()); // Could not open file
        return false;
    }
    QByteArray save_data = load_file.readAll();         // Read file
    QJsonDocument load_doc(save_format == JSON
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));
    this->read(load_doc.object());                      // Read this object
    m_full_path = (full_path.find(".json") != std::string::npos ||
            full_path.find(".dat") != std::string::npos)
            ? full_path
            : load_file.fileName().toStdString();;  // Set path to saved path
    return true;
}
