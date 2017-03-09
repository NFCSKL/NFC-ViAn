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
 * @brief FileHandlerTest::project_handling_test
 * not much to test currently, will be extended
 * with furter implementation of project manipulation
 */
void filehandlertest::project_handling_test(){
    FileHandler* fh = new FileHandler();
    Project* proj =  fh->create_project("/TEST_PROJ");
    std::string v1 = "video1.txt"; // names for testfiles
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";
    ID vid1 = fh->create_file(v1, proj->m_dir);  // create testfiles
    ID vid2 = fh->create_file(v2, proj->m_dir);
    ID vid3 = fh->create_file(v3, proj->m_dir);

    fh->add_video(proj, fh->get_file(vid1)); //add tesfiles as videos to project.
    fh->add_video(proj, fh->get_file(vid2));
    fh->add_video(proj, fh->get_file(vid3));
    fh->save_project(proj);
    //check file contents
    fh->load_project(fh->get_dir(proj->m_dir));
    //check project contentss
    fh->delete_file(vid1);
    fh->delete_file(vid2);
    fh->delete_file(vid3);
    fh->delete_project(proj);
}
//
// Test that creation and deletion of directories are working correctly.
//
void filehandlertest::directory_test(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + std::string("/TEST_PROJ");
    int err = fh->create_directory(dirpath);
    QCOMPARE(err, 0);
    QCOMPARE(fh->delete_directory(dirpath), 0);
}
// fileTest tests following functions, see filehandler.h
//
// ID create_directory(std::string path);
// int delete_directory(std::string dirpath);
// ID create_file(std::string filename, ID dirID);
// int delete_file(ID id);
// void write_file(ID id, std::string text);

void filehandlertest::file_test(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + "/TEST_MAP";
    std::string filename = "filetest.txt";
    ID dir = fh->create_directory(dirpath);
    ID fileID  = fh->create_file(filename,dir); //Create file ID = i    (1)
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
    QCOMPARE(fh->delete_directory(dirpath), 0); //delete directory

}

