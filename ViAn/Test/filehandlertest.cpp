#include "filehandlertest.h"

/**
 * @brief filehandlertest::filehandlertest
 * @param parent
 */
FileHandlerTest::FileHandlerTest(QObject *parent) : QObject(parent){

}
/**
 * @brief FileHandlerTest::projectHandlingTest
 * Tests project creation and deletion.
 * TODO : tests project write to file and read from file
 */
void FileHandlerTest::project_handling_test(){
    FileHandler* fh = new FileHandler();
    Project* proj =  fh->create_project("TEST_PROJ");
    //fh->save_project(proj);
    //check file contents
    Project* proj2 = fh->load_project(fh->work_space->filePath("TEST_PROJ").toStdString());
    QVERIFY(fh->proj_equals(*proj2,*proj));
    QCOMPARE(fh->delete_project(proj), 0);
}
/**
 * @brief filehandlertest::directoryTest
 * Test directory creation and deletion
 */
void FileHandlerTest::directory_test(){
    FileHandler* fh = new FileHandler();
    QString dirpath = QString::fromStdString(fh->work_space->filePath("TEST_PROJ").toStdString());
    ID id = fh->create_directory(dirpath);
    QCOMPARE(fh->last_error, 0);
    QCOMPARE(fh->delete_directory(id), 0);
}
/**
 * @brief filehandlertest::fileTest
 *  Tests file creation, deletion, writing and reading.
 */

void FileHandlerTest::file_test(){
    FileHandler* fh = new FileHandler();
    QString dirpath = QString::fromStdString(fh->work_space->filePath("TEST_PROJ").toStdString());
    QString filename = QString::fromStdString("filetest.txt");
    ID dir = fh->create_directory(dirpath);
    ID fileID  = fh->create_file(filename, fh->get_dir(dir)); //Create file ID = i    (1)

    QCOMPARE(fh->get_dir(dir)->absolutePath(), dirpath);
    QCOMPARE(fh->get_dir(dir)->filePath(filename), dirpath + "/" + filename);

    ID fileID2  = fh->create_file(filename,fh->get_dir(dir)); // Create file ID = i+1 (2)
    QCOMPARE(fileID2 - fileID, 1);                         // check if 1 && 2 is true
    QVERIFY(fh->get_file(fileID) == fh->get_file(fileID2)); //check that (1), (2) point to same file.

    // TEST FILE WRITE AND FILE READ
    fh->write_file(fileID, filename);   //write to file "filetest.txt"
    //read and check if file contains filename"
    QString readText;
    fh->read_file(fileID, readText, 1); // read from file
    QCOMPARE(readText, filename); // d = 0 if test passes
    //delete file and check if it is not there
    QCOMPARE(fh->delete_file(fileID), 0); // check that delete was done correctly
    QCOMPARE(fh->delete_directory(dir), 0); //delete directory
}

