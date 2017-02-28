#ifndef TESTSUITE_FILEHANDLER_H
#define TESTSUITE_FILEHANDLER_H
#include "filehandler.h"
#include "project.h"
#define VAR_VALUES 0
//#define VAR_VALUES 1 //Uncomment this line for more precise information
                       //Which function failed
int runTestSuite();
int directoryTest(FileHandler* fileHandler, Project* proj);
int fileTest(FileHandler* fileHandler, Project* proj);
int projectHandlingTest(FileHandler* fileHandler, Project* proj);
int readWriteTest(FileHandler* fileHandler, Project* proj);
#endif // TESTSUITE_FILEHANDLER_H
