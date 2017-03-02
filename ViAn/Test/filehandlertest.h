#ifndef FILEHANDLERTEST_H
#define FILEHANDLERTEST_H
#include "Filehandler/filehandler.h"
#include "Filehandler/project.h"
#include <QTest>
#include <QObject>


class filehandlertest : public QObject
{
    Q_OBJECT

public:
    explicit filehandlertest(QObject *parent = 0);
private slots:
    void directoryTest();
    void fileTest();
    void projectHandlingTest();

};

#endif // FILEHANDLER_TESTSUITE_H
