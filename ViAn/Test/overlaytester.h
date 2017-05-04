#ifndef OVERLAYTESTER_H
#define OVERLAYTESTER_H

#include "GUI/mainwindow.h"

class OverlayTester {
public:
    OverlayTester();
    void exec();
private:
    MainWindow* main = new MainWindow();
};

#endif // OVERLAYTESTER_H
