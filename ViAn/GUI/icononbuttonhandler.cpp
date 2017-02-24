#include "icononbuttonhandler.h"

/**
 * @brief IconOnButtonHandler::IconOnButtonHandler
 * Constructor
 */
IconOnButtonHandler::IconOnButtonHandler(){
    QPixmap errorPixmap(":/error.png");
    QIcon errorIcon(errorPixmap);
    iconList[errorIconNumber] = errorIcon;
    QPixmap playPixmap(":/play.png");
    QIcon playIcon(playPixmap);
    iconList[playIconNumber] = playIcon;
    QPixmap pausePixmap(":/pause.png");
    QIcon pauseIcon(pausePixmap);
    iconList[pauseIconNumber] = pauseIcon;
    QPixmap stopPixmap(":/stop.png");
    QIcon stopIcon(stopPixmap);
    iconList[stopIconNumber] = stopIcon;
    QPixmap nextPixmap(":/next_frame.png");
    QIcon nextIcon(nextPixmap);
    iconList[nextIconNumber] = nextIcon;
    QPixmap previousPixmap(":/previous_frame.png");
    QIcon previousIcon(previousPixmap);
    iconList[previousIconNumber] = previousIcon;
}

/**
 * @brief IconOnButtonHandler::~IconOnButtonHandler
 * Destructor
 */
IconOnButtonHandler::~IconOnButtonHandler(){

}
/**
 * @brief IconOnButtonHandler::set_pictures_to_buttons
 * adds icons to all the buttons.
 * @param ui the window you want to add the icons to
 */
void IconOnButtonHandler::set_pictures_to_buttons(Ui::MainWindow *ui){
    ui->playButton->setIcon(iconList[playIconNumber]);
    ui->pauseButton->setIcon(iconList[pauseIconNumber]);
    ui->stopButton->setIcon(iconList[stopIconNumber]);
    ui->nextFrameButton->setIcon(iconList[nextIconNumber]);
    ui->previousFrameButton->setIcon((iconList[previousIconNumber]));
}

/**
 * @brief IconOnButtonHandler::getIcon
 *  if you send in a non valid numer you get an error image
 * @param iconNumber the number of the icon
 * @return the icon associated with the numer
 */
QIcon IconOnButtonHandler::getIcon(int iconNumber){
    if(iconNumber < numerOfIcons){
        return iconList[iconNumber];
    } else return iconList[errorIconNumber];
}
/**
 * @brief IconOnButtonHandler::setIcon
 * if you send in a non valid numer you get an error image
 * @param iconNumber
 * @param button
 */
void IconOnButtonHandler::setIcon(int iconNumber, QPushButton *button){
     if(iconNumber < numerOfIcons){
        button->setIcon(iconList[iconNumber]);
     }else button->setIcon(iconList[errorIconNumber]);
}
