#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
/**
 * @brief MainWindow::MainWindow
 * Constructor
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

    setShortcuts();
    /**
     * How to get a picture
     */
    /*QImage image;
    image.load("<searchPath>");
    ui->videoFrame->setPixmap(QPixmap::fromImage(image));
    ui->videoFrame->show();*/

}

/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow()
{
    delete iconOnButtonHandler;
    delete ui;
}

void MainWindow::setShortcuts(){
    ui->actionExit->setShortcut(tr("Ctrl+e"));
}

/**
 * @brief MainWindow::on_playButton_clicked
 * The button supposed to play the video
 */
void MainWindow::on_playButton_clicked()
{
    iconOnButtonHandler->setIcon("pause", ui->playButton);//changes the icon on the play button to a pause-icon
}

/**
 * @brief MainWindow::on_pauseButton_clicked
 * The button supposed to pause the video
 */
void MainWindow::on_pauseButton_clicked()
{

}


/**
 * @brief MainWindow::on_stopButton_clicked
 * The button supposed to stop the video
 */
void MainWindow::on_stopButton_clicked()
{
    iconOnButtonHandler->setIcon("play", ui->playButton);//changes the icon on the play button to a play-icon
}

/**
 * @brief MainWindow::on_videoSlider_valueChanged
 * Update the slider to where the mouse is
 * @param newPos current position of the slider
 */
void MainWindow::on_videoSlider_valueChanged(int newPos)
{
    // Make slider to follow the mouse directly and not by pageStep steps
    Qt::MouseButtons btns = QApplication::mouseButtons();
    QPoint localMousePos = ui->videoSlider->mapFromGlobal(QCursor::pos());
    bool clickOnSlider = (btns & Qt::LeftButton) &&
                         (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
                          localMousePos.x() < ui->videoSlider->size().width() &&
                          localMousePos.y() < ui->videoSlider->size().height());
    if (clickOnSlider)
    {
        // Attention! The following works only for Horizontal, Left-to-right sliders
        float posRatio = localMousePos.x() / (float )ui->videoSlider->size().width();
        int sliderRange = ui->videoSlider->maximum() - ui->videoSlider->minimum();
        int sliderPosUnderMouse = ui->videoSlider->minimum() + sliderRange * posRatio;
        if (sliderPosUnderMouse != newPos)
        {
            ui->videoSlider->setValue(sliderPosUnderMouse);
            return;
        }
    }
}



void MainWindow::on_actionExit_triggered()
{
    QMessageBox exitBox;
    exitBox.setWindowTitle("Exit?");
    exitBox.setText("Do you really wanna exit?");
    exitBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    exitBox.setDefaultButton(QMessageBox::No);
    int ret = exitBox.exec();
    //exitBox.setFixedSize(500,200);

    switch(ret){
    case QMessageBox::Yes:
        //Yes was clicked
        QCoreApplication::quit();
        break;
    case QMessageBox::No:
        //No was clicked
        break;
    default:
        //Should never be reached
        break;
    }
}
