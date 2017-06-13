#ifndef SAVABLE_H
#define SAVABLE_H
#include <QJsonObject>
/**
 * @brief The Saveable class
 * The saveable class is an abstract class
 * used to guarantee an objects ability to be saved
 * as a json or binary in filehandler.
 */
typedef int ID;
class Saveable
{
    enum SAVE_FORMAT {JSON, BINARY};    // Formats supported by save_project

public:
    const SAVE_FORMAT save_format = BINARY;
    std::string save_name;
    Saveable();

    void save();
    void load();
    // Saveable methods
    Saveable* load_saveable(Saveable *saveable, std::string full_path, SAVE_FORMAT save_form);
    bool save_saveable(Saveable* saveable, ID dir_id, SAVE_FORMAT save_format);
    virtual ~Saveable();
    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;
};

#endif // SAVABLE_H
