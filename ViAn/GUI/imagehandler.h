#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

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
