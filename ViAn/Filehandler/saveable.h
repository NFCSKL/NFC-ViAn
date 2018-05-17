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
#include "writeable.h"
/**
 * @brief The Saveable class
 * The saveable class is an abstract class
 * used to guarantee an objects ability to be saved
 * as a json or binary in filehandler.
 */
class Saveable : public Writeable
{
private:
    std::string m_full_path = ""; // Used to save complete saved path, i.e /Dir/Path/file.ending
public:

    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project
    static const SAVE_FORMAT DEFAULT_SAVE_FORMAT = JSON; // Default format
public:
    Saveable();
    virtual ~Saveable();    
    virtual bool save_saveable(const std::string& file_name, const std::string& dir_path, const SAVE_FORMAT& save_format);
    virtual bool load_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);    
    virtual bool save_saveable(const std::string &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool delete_saveable(); // Deletes saved file := m_full_path
    virtual bool delete_saveable(const std::string &full_path);

    virtual std::string full_path() const;
};

#endif // SAVABLE_H
