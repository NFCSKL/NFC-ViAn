#ifndef OVERLAYTESTER_H
#define OVERLAYTESTER_H

#include "GUI/mainwindow.h"
#include "Video/video_player.h"

class OverlayTester {
public:
    OverlayTester();
    void exec();
private:
    MainWindow* main = new MainWindow();
    video_player* mvideo;
};

#endif // OVERLAYTESTER_H
