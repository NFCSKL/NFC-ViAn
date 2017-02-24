#ifndef ICONONBUTTONHANDLER_H
#define ICONONBUTTONHANDLER_H
#define errorIconNumber 0
#define playIconNumber 1
#define pauseIconNumber 2
#define stopIconNumber 3
#define nextIconNumber 4
#define previousIconNumber 5
#define numberOfIcons 6

#include "ui_mainwindow.h"
#include <string>

class IconOnButtonHandler{
public:
    explicit IconOnButtonHandler();
    ~IconOnButtonHandler();
    void set_pictures_to_buttons(Ui::MainWindow *ui);
    QIcon getIcon(std::string icon);
    void setIcon(std::string icon, QPushButton *button);
    std::map <std::string, QIcon> iconMap;


private slots:

    QIcon iconList [numberOfIcons];
private:

};


#endif // ICONONBUTTONHANDLER_H
