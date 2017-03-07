#include "inputwindow.h"
#include "ui_inputwindow.h"

/**
 * @brief inputwindow::inputwindow
 * Constructor
 * @param mainWindow
 * @param action the action performed
 * @param infoText text to place on the top of this window
 * @param parent
 */
inputwindow::inputwindow( MainWindow *mainWindow, ACTION action, QString infoText = "Name:", QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputwindow)
{
    this->mainWindow = mainWindow;
    this->action = action;
    ui->setupUi(this);
}
/**
 * @brief inputwindow::~inputwindow
 * Destructor
 */
inputwindow::~inputwindow()
{
    delete ui;
}
/**
 * @brief inputwindow::on_okButton_clicked
 * sends the input text back to mainWindow
 */
void inputwindow::on_okButton_clicked() {
    if(!ui->input->text().isEmpty()){
        QString inputText = ui->input->text();
        mainWindow->inputSwitchCase(this->action, inputText);
    }
}
/**
 * @brief inputwindow::on_cancleButton_clicked
 * tells mainWindow that we dont want to do this
 */
void inputwindow::on_cancleButton_clicked() {
    QString noText = "NoName";
    mainWindow->inputSwitchCase(CANCELL, noText);
}
