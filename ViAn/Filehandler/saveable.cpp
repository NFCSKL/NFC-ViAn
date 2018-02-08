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
    dir.mkpath(QString::fromStdString(dir_path)); // Make required directory path
    return save_saveable(dir_path + file_name, save_format); // Call with full path dir + "/file_name"
}

/**
 * @brief Saveable::save_saveable
 * @param full_path
 * @param save_format
 * @return true if successfull, false else
 */
bool Saveable::save_saveable(const std::string &full_path, const Saveable::SAVE_FORMAT &save_format) {
    m_full_path = full_path;
    qDebug() << "full_path in savable" << QString::fromStdString(full_path);
    QFile save_file(QString::fromStdString(full_path));
    if(!save_file.open(QIODevice::WriteOnly)){  // Attempt to open full_path
        qDebug() << "Couldn't open save file", save_file.fileName().toStdString().c_str();   // Send warning if unsuccessful
        return false;
    }
    QJsonObject json_saveable;                  // Data to be saved
    this->write(json_saveable);                 // Write data to be saved, OBS! Implement this when inheriting
    QJsonDocument save_doc(json_saveable);      // File to write to
    save_file.write(save_format == JSON         // Choose written format
            ? save_doc.toJson()
            : save_doc.toBinaryData());

    return true;
}

/**
 * @brief Saveable::delete_saveable
 * @return
 * Delete previously set path m_full_path
 * m_full_path is set by both save_saveable and load saveable
 */
bool Saveable::delete_saveable()
{
    QFile file(QString::fromStdString(m_full_path));
    if(file.exists()) return file.remove();
    qDebug() << "Couldn't delete file: " + QString::fromStdString(m_full_path);
    return false;
}

/**
 * @brief Saveable::load_savable
 * @param full savable path
 * @param save_format
 * @return loaded Savable
 * Loads saveable from binary or json file.
 */
bool Saveable::load_saveable(const std::string& full_path, const SAVE_FORMAT& save_format){
    m_full_path = full_path;
    QFile load_file(QString::fromStdString(full_path));
    if (!load_file.open(QIODevice::ReadOnly)) {     // Attempt to open file
        qWarning("Couldn't open load file %s ",
                 load_file.fileName().toStdString().c_str()); // Could not open file
        // Ur code handling failure
        return false;
    }
    QByteArray save_data = load_file.readAll();         // Read file to be loaded
    QJsonDocument load_doc(save_format == JSON          // Decide format to decode to
        ? QJsonDocument::fromJson(save_data)
        : QJsonDocument::fromBinaryData(save_data));
    this->read(load_doc.object());                      // Read data to be loaded, OBS! Implement this when inheriting

    return true;
}
