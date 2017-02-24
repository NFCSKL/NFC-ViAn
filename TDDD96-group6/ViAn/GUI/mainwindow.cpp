#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    delete ui;
}

/**
 * @brief MainWindow::on_playButton_clicked
 * The leftmost button supposed to play the video
 */
void MainWindow::on_playButton_clicked()
{

}

/**
 * @brief MainWindow::on_pauseButton_clicked
 * The middle button supposed to pause the video
 */
void MainWindow::on_pauseButton_clicked()
{

}


/**
 * @brief MainWindow::on_stopButton_clicked
 * The rightmost button supposed to stop the video
 */
void MainWindow::on_stopButton_clicked()
{

}


