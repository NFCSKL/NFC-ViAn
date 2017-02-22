#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    /*QPixmap pixmap("/home/krini678/Desktop/stop-button.jpg");
    QIcon stopIcon(pixmap);
    ui->stopButton->setIcon(stopIcon);*/

    //How to get a picture
    QImage image;
    image.load("");
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->show();

}


MainWindow::~MainWindow()
{
    delete ui;
}

//Left button
void MainWindow::on_playButton_clicked()
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, "hej");
    ui->treeWidget->addTopLevelItem(treeItem);
}

//Middle button
void MainWindow::on_pauseButton_clicked()
{

}

//Right button
void MainWindow::on_stopButton_clicked()
{
    ui->horizontalSlider->setValue(0);
}


