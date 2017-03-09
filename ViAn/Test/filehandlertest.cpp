#include "filehandlertest.h"
/**
 * @brief runTestSuite
 * @return all pass errorcode,
 * outputs information about which tests passed if
 * PRINTS set in header to terminal
 *
 */
//void filehandler_testsuite::runTestSuite(){
//    FileHandler* fileHandler = new FileHandler();
//    std::string dirpath = WORKSPACE;
//    Project* proj  = fileHandler->createProject("POI_PROJ");
//    directoryTest(fileHandler, proj);
//    fileTest(fileHandler, proj);
//    projectHandlingTest(fileHandler, proj);
//}

//
// Tests that creation, manipulation and deletion of
// projects functions as intended
//
filehandlertest::filehandlertest(QObject *parent) : QObject(parent){

}
/**
 * @brief FileHandlerTest::projectHandlingTest
 * not much to test currently, will be extended
 * with furter implementation of project manipulation
 */
void filehandlertest::projectHandlingTest(){
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
//    Project* proj2 = fh->loadProject(fh->getDir(proj->m_dir));
//    QVERIFY(*proj2 == *proj);
    //check project contentss
    fh->delete_file(vid1);
    fh->delete_file(vid2);
    fh->delete_file(vid3);
    QCOMPARE(fh->delete_project(proj), 0);
}
//
// Test that creation and deletion of directories are working correctly.
//
void filehandlertest::directoryTest(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + std::string("/TEST_PROJ");
    ID id = fh->create_directory(dirpath);
    QCOMPARE(fh->lastError, 0);
    QCOMPARE(fh->delete_directory(id), 0);
}
// fileTest tests following functions, see filehandler.h
//
// ID createDirectory(std::string path);
// int deleteDirectory(std::string dirpath);
// ID createFile(std::string filename, ID dirID);
// int deleteFile(ID id);
// void writeFile(ID id, std::string text);

void filehandlertest::fileTest(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + "/TEST_MAP";
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
    fh->read_file(fileID,1, readText); // read from file
    QCOMPARE(readText, filename); // d = 0 if test passes
    //delete file and check if it is not there
    QCOMPARE(fh->delete_file(fileID), 0); // check that delete was done correctly
    QCOMPARE(fh->delete_directory(dir), 0); //delete directory

}

