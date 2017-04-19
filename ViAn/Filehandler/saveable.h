#ifndef SAVABLE_H
#define SAVABLE_H
#include <QJsonObject>
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
