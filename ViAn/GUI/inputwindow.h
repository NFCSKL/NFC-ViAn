#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QString>
#include "mainwindow.h"
#include "action.h"

class MainWindow;
namespace Ui {
class inputwindow;
}

class inputwindow : public QWidget
{
    Q_OBJECT

public:
    explicit inputwindow(MainWindow *mainWindow, ACTION action, QString infoText, QWidget *parent = 0);
    ~inputwindow();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::inputwindow *ui;
    MainWindow *mainWindow;
    ACTION action;

};

#endif // INPUTWINDOW_H
