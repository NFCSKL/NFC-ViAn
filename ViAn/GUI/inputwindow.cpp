#include "inputwindow.h"
#include "ui_inputwindow.h"

inputwindow::inputwindow(QString infoText = "Name:", QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputwindow)
{
    ui->setupUi(this);
}

inputwindow::~inputwindow()
{
    delete ui;
}

QString inputwindow::getInputString(){
    while(!ui->okButton->isDown()){

    }
    return ui->input->text();
}
