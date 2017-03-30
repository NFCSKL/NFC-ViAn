#include "inputwindow.h"
#include "ui_inputwindow.h"
#include <QShortcut>

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
    ui(new Ui::inputwindow) {
    this->mainWindow = mainWindow;
    this->action = action;
    ui->setupUi(this);
    ui->infoLabel->setText(infoText);
    QShortcut *ok_shortcut = new QShortcut(QKeySequence(tr("Return")), this);
    QObject::connect(ok_shortcut, SIGNAL(activated()), this, SLOT(on_okButton_clicked()));
    QShortcut *cancel_shortcut = new QShortcut(QKeySequence(tr("Esc")), this);
    QObject::connect(cancel_shortcut, SIGNAL(activated()), this, SLOT(on_cancelButton_clicked()));
}
/**
 * @brief inputwindow::~inputwindow
 * Destructor
 */
inputwindow::~inputwindow() {
    delete ui;
}
/**
 * @brief inputwindow::on_okButton_clicked
 * sends the input text back to mainWindow
 */
void inputwindow::on_okButton_clicked() {
    if(!ui->input->text().isEmpty()) {
        QString inputText = ui->input->text();
        mainWindow->input_switch_case(this->action, inputText);
    }
}
/**
 * @brief inputwindow::on_cancelButton_clicked
 * tells mainWindow that we dont want to do this
 */
void inputwindow::on_cancelButton_clicked() {
    QString noText = "NoName";
    mainWindow->input_switch_case(CANCEL, noText);
}
