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
void filehandlertest::projectHandlingTest(){
    FileHandler* fh = new FileHandler();
    Project* proj =  fh->createProject("TEST_PROJ");
    std::string v1 = "video1.txt"; // names for testfiles
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";
    ID vid1 = fh->createFile(v1, proj->files->dir);  // create testfiles
    ID vid2 = fh->createFile(v2, proj->files->dir);
    ID vid3 = fh->createFile(v3, proj->files->dir);

    fh->addVideo(proj, fh->getFile(vid1)); //add tesfiles as videos to project.
    fh->addVideo(proj, fh->getFile(vid2));
    fh->addVideo(proj, fh->getFile(vid3));

    fh->saveProject(proj);
    //check file contents
//    Project* proj2 = fh->loadProject(fh->getDir(proj->m_dir));
//    QVERIFY(*proj2 == *proj);
    //check project contentss
    fh->deleteFile(vid1);
    fh->deleteFile(vid2);
    fh->deleteFile(vid3);
    QCOMPARE(fh->deleteProject(proj), 0);
}
/**
 * @brief filehandlertest::directoryTest
 * Test directory creation and deletion
 */
void filehandlertest::directoryTest(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + std::string("/TEST_PROJ");
    ID id = fh->createDirectory(dirpath);
    QCOMPARE(fh->lastError, 0);
    QCOMPARE(fh->deleteDirectory(id), 0);
}
/**
 * @brief filehandlertest::fileTest
 *  Tests file creation, deletion, writing and reading.
 */

void filehandlertest::fileTest(){
    FileHandler* fh = new FileHandler();
    std::string dirpath = std::string(WORKSPACE) + "/TEST_MAP";
    std::string filename = "filetest.txt";
    ID dir = fh->createDirectory(dirpath);
    ID fileID  = fh->createFile(filename,dir); //Create file ID = i    (1)
    QCOMPARE(fh->getDir(dir), dirpath);
    QCOMPARE(fh->getFile(fileID), dirpath + "/" + filename);

    ID fileID2  = fh->createFile(filename,dir); // Create file ID = i+1 (2)
    QCOMPARE(fileID2 - fileID, 1);                         // check if 1 && 2 is true
    QVERIFY(fh->getFile(fileID) == fh->getFile(fileID2)); //check that (1), (2) point to same file.

    // TEST FILE WRITE AND FILE READ
    fh->writeFile(fileID, filename);   //write to file "filetest.txt"
    //read and check if file contains filename"
    std::string readText;
    fh->readFile(fileID,1, readText); // read from file
    QCOMPARE(readText, filename); // d = 0 if test passes
    //delete file and check if it is not there
    QCOMPARE(fh->deleteFile(fileID), 0); // check that delete was done correctly
    QCOMPARE(fh->deleteDirectory(dir), 0); //delete directory

}

