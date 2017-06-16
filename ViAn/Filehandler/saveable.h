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
/**
 * @brief The Saveable class
 * The saveable class is an abstract class
 * used to guarantee an objects ability to be saved
 * as a json or binary in filehandler.
 */
typedef int ID;

class Saveable
{
public:
    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project
    static const SAVE_FORMAT DEFAULT_SAVE_FORMAT = JSON;
    std::string m_full_path;
public:
    Saveable();
    virtual ~Saveable();
    // Saveable methods
    bool load_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);

    bool save_saveable(const std::string &file_name, const std::string &dir_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    bool save_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);



    virtual void delete_saveable();
    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;
};

#endif // SAVABLE_H
