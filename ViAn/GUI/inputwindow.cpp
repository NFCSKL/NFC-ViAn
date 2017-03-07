#include "inputwindow.h"
#include "ui_inputwindow.h"

inputwindow::inputwindow( MainWindow *mainWindow, ACTION action, QString infoText = "Name:", QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputwindow)
{
    this->mainWindow = mainWindow;
    this->action = action;
    ui->setupUi(this);
}

inputwindow::~inputwindow()
{
    delete ui;
}

void inputwindow::on_okButton_clicked(){
    if(!ui->input->text().isEmpty()){
        QString inputText = ui->input->text();
        mainWindow->inputSwitchCase(inputText, this->action);
    }
}
