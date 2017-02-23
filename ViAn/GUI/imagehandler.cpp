#include "imagehandler.h"

ImageHandler::ImageHandler(){
    QPixmap playPixmap(":/play.png");
    QIcon playIcon(playPixmap);
    iconList[play_video] = playIcon;
    QPixmap pausePixmap(":/pause.png");
    QIcon pauseIcon(pausePixmap);
    iconList[pause_video] = pauseIcon;
    QPixmap stopPixmap(":/stop.png");
    QIcon stopIcon(stopPixmap);
    iconList[stop_video] = stopIcon;
    QPixmap nextPixmap(":/next_frame.png");
    QIcon nextIcon(nextPixmap);
    iconList[next_frame] = nextIcon;
    QPixmap previousPixmap(":/previous_frame.png");
    QIcon previousIcon(previousPixmap);
    iconList[previous_frame] = previousIcon;
}

ImageHandler::~ImageHandler(){
    for (unsigned int i = 0; i < sizeof(iconList); i++){
        delete &iconList[i];
    }
}

void ImageHandler::set_pictures_to_buttons(Ui::MainWindow *ui){
    //QIcon iconList [3];

    ui->playButton->setIcon(iconList[play_video]);
    ui->pauseButton->setIcon(iconList[pause_video]);
    ui->stopButton->setIcon(iconList[stop_video]);
    ui->nextFrameButton->setIcon(iconList[next_frame]);
    ui->previousFrameButton->setIcon((iconList[previous_frame]));
}
