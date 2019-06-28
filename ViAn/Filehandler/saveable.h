#ifndef SAVABLE_H
#define SAVABLE_H

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
    QString m_full_path = ""; // Used to save complete saved path, i.e /Dir/Path/file.ending
public:

    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project
    static const SAVE_FORMAT DEFAULT_SAVE_FORMAT = JSON; // Default format
public:
    Saveable();
    virtual ~Saveable();    
    virtual bool save_saveable(const QString &file_name, const QString &dir_path, const SAVE_FORMAT& save_format);
    virtual bool load_saveable(const QString &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool save_saveable(const QString &full_path, const SAVE_FORMAT &save_format = DEFAULT_SAVE_FORMAT);
    virtual bool delete_saveable(); // Deletes saved file := m_full_path
    virtual bool delete_saveable(const QString &full_path);

    virtual QString full_path() const;
};

#endif // SAVABLE_H
