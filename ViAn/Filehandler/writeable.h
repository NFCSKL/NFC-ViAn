#ifndef WRITEABLE_H
#define WRITEABLE_H

#include <QJsonObject>

class Writeable
{
public:
    Writeable();
    virtual ~Writeable();

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
};

#endif // WRITEABLE_H
