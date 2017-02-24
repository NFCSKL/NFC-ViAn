#include "testsuite_filehandler.h"

int runTestSuite(){
    int dirTest,fileTest,projectTest;
    dirTest = 0;
    fileTest = 0;
    projectTest = 0;
    FileHandler* fileHandler = new FileHandler();
    std::string dirpath = "C:/Programmering";
    Project* proj  = fileHandler->createProject("POI_PROJ");
    dirTest = directoryTest(fileHandler, proj);
    fileTest = fileTest(fileHandler, proj);
    projectTest  = projectHandlingTest(fileHandler, proj);

    if(VAR_VALUES)
        std::cout << "dirtest: " << (dirTest != 0) <<std::endl<<
        "\nfiletest: " << (b != 0) << "\nprojecttest: " << c << std::endl;

    return dirTest+fileTest+projectTest!=0;
}

int projectHandlingTest(FileHandler* fileHandler, Project* proj){

    std::string v1 = "video1.txt";
    std::string v2 = "video2.txt";
    std::string v3 = "video3.txt";

    ID vid1 = fileHandler->createFile(v1, proj->m_dir);
    ID vid2 = fileHandler->createFile(v2, proj->m_dir);
    ID vid3 = fileHandler->createFile(v3, proj->m_dir);

    fileHandler->addVideo(proj, fileHandler->getFile(vid1));
    fileHandler->addVideo(proj, fileHandler->getFile(vid2));
    fileHandler->addVideo(proj, fileHandler->getFile(vid3));
    readWriteTest(fileHandler, proj);


    return 0;
}
int readWriteTest(FileHandler* fileHandler, Project* proj){
    fileHandler->saveProject(proj);
    //fileHandler->loadProject(fileHandler->getDir(proj->m_dir));
    return 0;
}

int directoryTest(FileHandler* fileHandler, Project* proj){
    std::string dir = "C:/Programmering/DIR_TEST";
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
//ID createDirectory(std::string path);
// int deleteDirectory(std::string dirpath);
// ID createFile(std::string filename, ID dirID);
// int deleteFile(ID id);
// void writeFile(ID id, std::string text);

int fileTest(FileHandler* fileHandler,Project* proj){
    int a,b,c,d,e,f;
    std::string dirpath = "C:/Programmering/FILE_TEST";
    std::string filename = "filetest.txt";
    ID dir = fileHandler->createDirectory(dirpath);
    a = GetLastError();
    ID fileID  = fileHandler->createFile(filename,dir); //Create file
    ID fileID2  = fileHandler->createFile(filename,dir);
    b = (1 == fileID - fileID2);
    c = (fileHandler->getFile(fileID) != fileHandler->getFile(fileID2));
    fileHandler->writeFile(fileID, filename);//write to file ( "testtext")
    //read and check if file contains "testText"
    std::string readText;
    fileHandler->readFile(fileID,1, readText);
    d = (readText != filename);
    //delete file and check if it is not there
    //  e = fileHandler->deleteFile(fileID);
    // f = (fileHandler->deleteFile(fileID)== 0);

    fileHandler->deleteDirectory(dirpath);
    if(VAR_VALUES)
    {
        std::cout<< "ErrorCodes:" << std::endl << "\tcreate file " << a <<
                    std::endl <<"\tdifferent id for same file " << b << std::endl<<
                    std::endl <<"\tget same file from different id " << c << std::endl;
                    std::cout << "\t correct write to file:"<<d<< std::endl<<"\tdelete was done correctly: " << e << std::endl;
    }
    return a+b+c+d+e;
}

int projectHandlingTest();
