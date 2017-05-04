#ifndef OVERLAYTESTER_H
#define OVERLAYTESTER_H

#include "GUI/mainwindow.h"
#include "Video/video_player.h"

class OverlayIntegrationTest {
public:
    OverlayIntegrationTest();
    void exec();
private:
    void draw_pen();
    MainWindow* main = new MainWindow();
    video_player* mvideo;
};

#endif // OVERLAYTESTER_H
