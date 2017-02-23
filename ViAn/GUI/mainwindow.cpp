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

    QPixmap playPixmap(":/play.png");
    QIcon playIcon(playPixmap);
    ui->playButton->setIcon(playIcon);

    QPixmap pausePixmap(":/pause.png");
    QIcon pauseIcon(pausePixmap);
    ui->pauseButton->setIcon(pauseIcon);

    QPixmap stopPixmap(":/stop.png");
    QIcon stopIcon(stopPixmap);
    ui->stopButton->setIcon(stopIcon);


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


