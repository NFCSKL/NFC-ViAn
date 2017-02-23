#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>

using namespace std;
using namespace cv;

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
    play_video();
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

void MainWindow::play_video()
{
    VideoCapture video = video_player.get_video_from_file("seq_01.mp4");
    double fps = video.get(CV_CAP_PROP_FPS);


    Mat frame = video_player.play_frame(video);
    QImage dest((const uchar *) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    dest.bits();

    while(true) {
        ui->videoFrame->setPixmap(QPixmap::fromImage(dest));
        ui->videoFrame->show();
        std::chrono::milliseconds dura((int)(1000/fps));
        std::this_thread::sleep_for( dura );
        frame = video_player.play_frame(video);
        QImage dest((const uchar *) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        dest.bits();
    }

}


