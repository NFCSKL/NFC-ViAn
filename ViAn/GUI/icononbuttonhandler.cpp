#include "icononbuttonhandler.h"
#include <QMessageBox>
#include <iostream>
#include <string>

/**
 * @brief IconOnButtonHandler::IconOnButtonHandler
 * Constructor
 */

IconOnButtonHandler::IconOnButtonHandler(){
    try{
        QPixmap errorPixmap(":/error.png");
        QIcon errorIcon(errorPixmap);
        iconMap["error"] = errorIcon;
        //iconList[errorIconNumber] = errorIcon;

        QPixmap playPixmap(":/play.png");
        QIcon playIcon(playPixmap);
        iconMap["play"] = playIcon;
        //iconList[playIconNumber] = playIcon;

        QPixmap pausePixmap(":/pause.png");
        QIcon pauseIcon(pausePixmap);
        iconMap["pause"] = pauseIcon;
        //iconList[pauseIconNumber] = pauseIcon;

        QPixmap stopPixmap(":/stop.png");
        QIcon stopIcon(stopPixmap);
        iconMap["stop"] = stopIcon;
        //iconList[stopIconNumber] = stopIcon;

        QPixmap nextPixmap(":/next_frame.png");
        QIcon nextIcon(nextPixmap);
        iconMap["next"] = nextIcon;
        //iconList[nextIconNumber] = nextIcon;

        QPixmap previousPixmap(":/previous_frame.png");
        QIcon previousIcon(previousPixmap);
        iconMap["previous"] = previousIcon;
        //iconList[previousIconNumber] = previousIcon;

        /*QPixmap ePixmap(":/stopp.png");
        QIcon eIcon(ePixmap);
        iconMap["e"] = eIcon;*/
        //iconList[2] = eIcon;

    }catch (const std::out_of_range& e){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","List out of range!");
        messageBox.setFixedSize(500,200);
    }catch (const std::runtime_error& e){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","U suk!");
        messageBox.setFixedSize(500,200);
    }catch (const std::exception& e){
        std::cerr << "Caught exception \"" << e.what() << "\"\n";
    }catch (...){
        std::cerr << "ERRORSSS";
    }
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
    ui->playButton->setIcon(iconMap["play"]);
    ui->pauseButton->setIcon(iconMap["pause"]);
    ui->stopButton->setIcon(iconMap["stop"]);
    ui->nextFrameButton->setIcon(iconMap["next"]);
    ui->previousFrameButton->setIcon(iconMap["previous"]);
}

/**
 * @brief IconOnButtonHandler::getIcon
 *  if you send in a non valid number you get an error image
 * @param iconNumber the number of the icon
 * @return the icon associated with the number
 */
QIcon IconOnButtonHandler::getIcon(std::string icon){
    if(iconMap.find(icon) == iconMap.end()){
        return iconMap["error"];
    }else{
        return iconMap[icon];
    }
}
/**
 * @brief IconOnButtonHandler::setIcon
 * if you send in a non valid number you get an error image
 * @param iconNumber
 * @param button
 */
void IconOnButtonHandler::setIcon(std::string icon, QPushButton *button){
    if(iconMap.find(icon) == iconMap.end()){
        button->setIcon(iconMap["error"]);
    }else{
        button->setIcon(iconMap[icon]);
    }
}
