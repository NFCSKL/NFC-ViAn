#ifndef TEST_REPORT_GENERATOR_H
#define TEST_REPORT_GENERATOR_H
#include <Filehandler/filehandler.h>
#include <Filehandler/project.h>
#include <GUI/reportgenerator.h>
#include <QTest>
#include <QObject>

class test_report_generator : public QObject
{
    Q_OBJECT
public:
    explicit test_report_generator(QObject *parent = 0);

private:
    FileHandler* file_handler;
    Project* proj1;
    Project* proj2;
    ReportGenerator* report_generator1;
    ReportGenerator* report_generator2;

private slots:
    void test_init();
    void test_create_and_save_report();
    void test_create_list_of_names();

};

#endif // TEST_REPORT_GENERATOR_H
