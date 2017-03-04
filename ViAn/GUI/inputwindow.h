#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QString>

namespace Ui {
class inputwindow;
}

class inputwindow : public QWidget
{
    Q_OBJECT

public:
    explicit inputwindow(QString infoText, QWidget *parent = 0);
    ~inputwindow();
    QString getInputString();

private:
    Ui::inputwindow *ui;
};

#endif // INPUTWINDOW_H
