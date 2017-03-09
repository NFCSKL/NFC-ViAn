#ifndef ICONONBUTTONHANDLER_H
#define ICONONBUTTONHANDLER_H

#include "ui_mainwindow.h"
#include <string>


class IconOnButtonHandler{
public:
    explicit IconOnButtonHandler();
    ~IconOnButtonHandler();
    void set_pictures_to_buttons(Ui::MainWindow *ui);
    void set_icon(std::string icon, QPushButton *button);


private slots:
    std::map <std::string, QIcon> iconMap;

private:

};


#endif // ICONONBUTTONHANDLER_H
