#ifndef ICONONBUTTONHANDLER_H
#define ICONONBUTTONHANDLER_H

#include "ui_mainwindow.h"
#include <string>

using namespace std;

class IconOnButtonHandler{
public:
    explicit IconOnButtonHandler();
    ~IconOnButtonHandler();
    void set_pictures_to_buttons(Ui::MainWindow *ui);
    void setIcon(string icon, QPushButton *button);


private slots:
    map <string, QIcon> iconMap;

private:

};


#endif // ICONONBUTTONHANDLER_H
