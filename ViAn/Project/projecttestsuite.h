#ifndef PROJECTTESTSUITE_H
#define PROJECTTESTSUITE_H
//QT
#include <QJsonObject>
#include <QObject>
#include <QTest>
//vian
#include "project.h"
#include "videoproject.h"
#include "report.h"
/**
 * @brief The ProjectTestsuite class
 * This class tests the usage of Projects public interface.
 */
class ProjectTestsuite
{
    Project* m_proj;
public:
    explicit ProjectTestsuite(QObject *parent = 0);
private slots:
    // Project tests
    void init_project_test();

    void add_remove_vid_proj_test();

    void add_remove_report_test();

    void read_write_test();
    void save_load_test();
    void delete_files_test();



};

#endif // PROJECTTESTSUITE_H
