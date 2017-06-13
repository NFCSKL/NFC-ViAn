#ifndef SAVABLE_H
#define SAVABLE_H
#include <QJsonObject>
/**
 * @brief The Saveable class
 * The saveable class is an abstract class
 * used to guarantee an objects ability to be saved
 * as a json or binary in filehandler.
 */
class Saveable
{
public:
    std::string save_name;
    Saveable();

    virtual ~Saveable();
    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;
};

#endif // SAVABLE_H
