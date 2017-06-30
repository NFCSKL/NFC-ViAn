#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QProgressBar>

class StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    StatusBar();
    QProgressBar* ana_prog;
private:
    const int PROG_BAR_WIDTH = 200;
    const int PROG_BAR_HEIGHT = 15;

public slots:
    void on_set_status_bar(QString status, int timer = 2000);
    void add_analysis_bar();
    void remove_analysis_bar();
    void update_analysis_bar(int);
};

#endif // STATUSBAR_H
