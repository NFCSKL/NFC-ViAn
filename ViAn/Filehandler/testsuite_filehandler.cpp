#include "testsuite_filehandler.h"
//
// Unit tests for filehandler
// IN: No input required
// OUT: text output to terminal, holding info on which tests
// has passed.
int runTestSuite(){
    int dt,ft,pt;
    dt = 0;
    ft = 0;
    pt = 0;
    FileHandler* fileHandler = new FileHandler();
    std::string dirpath = "C:/Programmering";
    Project* proj  = fileHandler->createProject("POI_PROJ");
    dt = directoryTest(fileHandler, proj);
    ft = fileTest(fileHandler, proj);
    pt  = projectHandlingTest(fileHandler, proj);

    if(VAR_VALUES)
        std::cout << "dirtest: " << (dt != 0) <<std::endl<<
        "\nfiletest: " << (ft != 0) << "\nprojecttest: " << pt << std::endl;

    return dt+ft+pt!=0;
}
//
// Tests that creation, manipulation and deletion of
// projects functions as intended
//

int projectHandlingTest(FileHandler* fileHandler, Project* proj){

    std::string v1 = "video1.txt"; // names for testfiles
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";
    ID vid1 = fileHandler->createFile(v1, proj->m_dir);  // create testfiles
    ID vid2 = fileHandler->createFile(v2, proj->m_dir);
    ID vid3 = fileHandler->createFile(v3, proj->m_dir);

    fileHandler->addVideo(proj, fileHandler->getFile(vid1)); //add tesfiles as videos to project.
    fileHandler->addVideo(proj, fileHandler->getFile(vid2));
    fileHandler->addVideo(proj, fileHandler->getFile(vid3));
    //readWriteTest(fileHandler, proj); // test that saving and deleting a project and its files works as intended,
                                        // WILL ADD WITH PARSER FUNCTIONALITY

    return 0;
}

int readWriteTest(FileHandler* fileHandler, Project* proj){
    fileHandler->saveProject(proj);
    fileHandler->loadProject(fileHandler->getDir(proj->m_dir));
    return 0;
}
//
// Test that creation and deletion of directories are working correctly.
//
int directoryTest(FileHandler* fileHandler, Project* proj){
    std::string dir = "C:/DIR_TEST";
    fileHandler->createDirectory(dir);
    int a = GetLastError();
    fileHandler->deleteDirectory(dir);
    int b = GetLastError();
    if(VAR_VALUES){
        std::cout<< "ErrorCodes:" << std::endl << "\tCreate dir " << a <<
                std::endl << "\tDelete dir " << b << std::endl;
    }
    return a+b;
}
// fileTest tests following functions, see filehandler.h
//
// ID createDirectory(std::string path);
// int deleteDirectory(std::string dirpath);
// ID createFile(std::string filename, ID dirID);
// int deleteFile(ID id);
// void writeFile(ID id, std::string text);

int fileTest(FileHandler* fileHandler,Project* proj){
    int a,b,c,d,e,f,g; // test variables
    std::string dirpath = "C:/FILE_TEST";
    std::string filename = "filetest.txt";
    ID dir = fileHandler->createDirectory(dirpath);
    a = GetLastError();
    ID fileID  = fileHandler->createFile(filename,dir); //Create file ID = i    (1)
    ID fileID2  = fileHandler->createFile(filename,dir); // Create file ID = i+1 (2)
    b = (1 == fileID - fileID2);                         // check if 1 && 2 is true
    c = (fileHandler->getFile(fileID) != fileHandler->getFile(fileID2)); //check that (1), (2) point to same file.

    // TEST FILE WRITE AND FILE READ
    fileHandler->writeFile(fileID, filename);   //write to file "filetest.txt"
    //read and check if file contains filename"
    std::string readText;
    fileHandler->readFile(fileID,1, readText); // read from file
    d = (readText == filename); // d = 0 if test passes

    //delete file and check if it is not there
    e = fileHandler->deleteFile(fileID); // delete file
    f = (fileHandler->deleteFile(fileID)== 0); // check that delete was done correctly

    g = (fileHandler->deleteDirectory(dirpath) == 0); //delete directory

    if(VAR_VALUES)
    {
        std::cout<< "ErrorCodes:" << std::endl << "\tcreate file " << a <<
                    std::endl <<"\tdifferent id for same file " << b << std::endl<<
                    std::endl <<"\tget same file from different id " << c << std::endl;
                    std::cout << "\t correct write to file:"<<d<< std::endl<<"\tdelete was done correctly: " << e << std::endl;
    }
    return a+b+c+d+e+g;
}

int projectHandlingTest();
