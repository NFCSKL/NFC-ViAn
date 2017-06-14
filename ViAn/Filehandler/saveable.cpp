#include "saveable.h"
/**
 * @brief Saveable::~Saveable
 * virtual constructor
 */
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
 * @return Saves a Json file to provided directory
 */
bool Saveable::save_saveable(const std::string& dir_path, const SAVE_FORMAT& save_format){
    QFile save_file(save_format == JSON
                    ? QString::fromStdString(dir_path + ".json")
                    : QString::fromStdString(dir_path + ".dat"));
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
    return true;
}

/**
 * @brief Saveable::load_savable
 * @param full savable path
 * @param save_format
 * @return loaded Savable
 * Loads saveable from binary or json file.
 */
bool Saveable::load_saveable(const std::string& full_path, const SAVE_FORMAT& save_format){
    QFile load_file(save_format == JSON
        ? QString::fromStdString(full_path)
        : QString::fromStdString(full_path));
    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file %s. ", load_file.fileName().toStdString().c_str());
        return false;
    }
    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc(save_format == JSON
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));
    this->read(load_doc.object());
    return true;
}
