#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    StatusBar();

public slots:
    void on_set_status_bar(QString status, int timer = 2000);
};

#endif // STATUSBAR_H
