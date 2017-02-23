#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H
#define play_video 0
#define pause_video 1
#define stop_video 2
#define next_frame 3
#define previous_frame 4

#include "ui_mainwindow.h"

class ImageHandler{
public:
    explicit ImageHandler();
    ~ImageHandler();
    void set_pictures_to_buttons(Ui::MainWindow *ui);

private slots:
    QIcon iconList [5];
    //std::map iconMap;
private:

};


#endif // IMAGEHANDLER_H
