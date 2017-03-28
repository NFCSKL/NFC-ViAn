#include "filehandlertest.h"

/**
 * @brief filehandlertest::filehandlertest
 * @param parent
 */
filehandlertest::filehandlertest(QObject *parent) : QObject(parent){

}
/**
 * @brief FileHandlerTest::projectHandlingTest
 * Tests project creation and deletion.
 * TODO : tests project write to file and read from file
 */
void filehandlertest::project_handling_test(){
    FileHandler* fh = new FileHandler();
    Project* proj =  fh->create_project("TEST_PROJ");
    std::string v1 = "video1.txt"; // names for testfiles
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";
    ID vid1 = fh->create_file(v1, proj->files->dir);  // create testfiles
    ID vid2 = fh->create_file(v2, proj->files->dir);
    ID vid3 = fh->create_file(v3, proj->files->dir);
    fh->add_video(proj, fh->get_file(vid1)); //add tesfiles as videos to project.
    fh->add_video(proj, fh->get_file(vid2));
    fh->add_video(proj, fh->get_file(vid3));

    fh->save_project(proj);
    //check file contents
    Project* proj2 = fh->load_project("TEST_PROJ", fh->workSpace + "TEST_PROJ");   
    QVERIFY(fh->proj_equals(*proj2,*proj));

    //check project contentss
    fh->delete_file(vid1);
    fh->delete_file(vid2);
    fh->delete_file(vid3);
    QCOMPARE(fh->delete_project(proj), 0);
}
/**
 * @brief filehandlertest::directoryTest
 * Test directory creation and deletion
 */
void filehandlertest::directory_test(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = fh->workSpace + std::string("TEST_PROJ");
    ID id = fh->create_directory(dirpath);
    QCOMPARE(fh->lastError, 0);
    QCOMPARE(fh->delete_directory(id), 0);
}
/**
 * @brief filehandlertest::fileTest
 *  Tests file creation, deletion, writing and reading.
 */

void filehandlertest::file_test(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = fh->workSpace + "TEST_MAP";
    std::string filename = "filetest.txt";
    ID dir = fh->create_directory(dirpath);
    ID fileID  = fh->create_file(filename,dir); //Create file ID = i    (1)

    QCOMPARE(fh->get_dir(dir), dirpath);
    QCOMPARE(fh->get_file(fileID), dirpath + "/" + filename);

    ID fileID2  = fh->create_file(filename,dir); // Create file ID = i+1 (2)
    QCOMPARE(fileID2 - fileID, 1);                         // check if 1 && 2 is true
    QVERIFY(fh->get_file(fileID) == fh->get_file(fileID2)); //check that (1), (2) point to same file.

    // TEST FILE WRITE AND FILE READ
    fh->write_file(fileID, filename);   //write to file "filetest.txt"
    //read and check if file contains filename"
    std::string readText;
    fh->read_file(fileID, readText, 1); // read from file
    QCOMPARE(readText, filename); // d = 0 if test passes
    //delete file and check if it is not there
    QCOMPARE(fh->delete_file(fileID), 0); // check that delete was done correctly
    QCOMPARE(fh->delete_directory(dir), 0); //delete directory
}

