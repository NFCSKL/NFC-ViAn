#include "imagehandler.h"

ImageHandler::ImageHandler(){
    QPixmap playPixmap(":/play.png");
    QIcon playIcon(playPixmap);
    iconList[0] = playIcon;
    QPixmap pausePixmap(":/pause.png");
    QIcon pauseIcon(pausePixmap);
    iconList[1] = pauseIcon;
    QPixmap stopPixmap(":/stop.png");
    QIcon stopIcon(stopPixmap);
    iconList[2] = stopIcon;
}

ImageHandler::~ImageHandler(){
    for (int i = 0; i < sizeof(iconList); i++){
        delete &iconList[i];
    }
}

void ImageHandler::set_pictures_to_buttons(Ui::MainWindow *ui){
    //QIcon iconList [3];

    ui->playButton->setIcon(iconList[0]);

    ui->pauseButton->setIcon(iconList[1]);

    ui->stopButton->setIcon(iconList[2]);
}
