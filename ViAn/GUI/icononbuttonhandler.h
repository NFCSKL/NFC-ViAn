#ifndef ICONONBUTTONHANDLER_H
#define ICONONBUTTONHANDLER_H
#define errorIconNumber 0
#define playIconNumber 1
#define pauseIconNumber 2
#define stopIconNumber 3
#define nextIconNumber 4
#define previousIconNumber 5
#define numerOfIcons 6

#include "ui_mainwindow.h"

class IconOnButtonHandler{
public:
    explicit IconOnButtonHandler();
    ~IconOnButtonHandler();
    void set_pictures_to_buttons(Ui::MainWindow *ui);
    QIcon getIcon(int iconNumber);
    void setIcon(int iconNumber, QPushButton *button);

private slots:
    QIcon iconList [numerOfIcons];
private:

};


#endif // ICONONBUTTONHANDLER_H
