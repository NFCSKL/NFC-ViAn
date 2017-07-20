#ifndef SAVABLE_H
#define SAVABLE_H
#include <QJsonObject>
#include <iostream>
// QT
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
#include <QDebug>
/**
 * @brief The Saveable class
 * The saveable class is an abstract class
 * used to guarantee an objects ability to be saved
 * as a json or binary in filehandler.
 */
class Saveable
{
private:
    std::string m_full_path = "INVALID PATH"; // Used to save complete saved path, i.e /Dir/Path/file.ending
public:

    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project
    static const SAVE_FORMAT DEFAULT_SAVE_FORMAT = JSON; // Default format


public:
    Saveable();
    virtual ~Saveable();    

    virtual bool load_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool save_saveable(const std::string &file_name, const std::string &dir_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool save_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool delete_saveable(); // Deletes saved file := m_full_path

    /**
     * @brief read
     * @param json
     * Read is used for loading data, implement this for interiting class.
     * this function will be called to read when loading.
     */
    virtual void read(const QJsonObject& json) = 0;

    /**
     * @brief read
     * @param json
     * Read is used for saving data, implement this for interiting class.
     * this function will be called to read when saving.
     */
    virtual void write(QJsonObject& json) = 0 ;

    virtual std::string full_path() const;
};

#endif // SAVABLE_H
