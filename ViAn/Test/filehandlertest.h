#ifndef FILEHANDLERTEST_H
#define FILEHANDLERTEST_H
#include "Filehandler/filehandler.h"
#include "Filehandler/project.h"
#include <QTest>
#include <QObject>


class FileHandlerTest : public QObject
{
    Q_OBJECT

public:
    explicit FileHandlerTest(QObject *parent = 0);
private:
    FileHandler* file_handler;
    ID dir_id = -1;
    ID file_id = -1;
    Project* proj;
private slots:
    // Directory test
    void directory_create_test();
    void directory_delete_test();
    // Workspace test
    //void workspace_set_test();
    // File tests
    //void file_test();

    // File create and deletion tests
    void file_test_init(); // Create directory
    void file_create_test(); // Create file
    void file_delete_test(); // Delete file

    void file_create_delete_multiple(); // Create a number of files, check ids and then delete them
    // File read and write tests
    void file_read_write_init();
    void file_read_write_test();
    void file_read_write_option_test();
    void file_read_lines_test();
    void file_read_write_cleanup();
    // Project tests
    //void project_init_tests();
    void project_test_init();
    void project_create_delete_test();
    void project_save_load_test();
    void project_add_remove_items();
    void project_cleanup();

};

#endif // FILEHANDLER_TESTSUITE_H
