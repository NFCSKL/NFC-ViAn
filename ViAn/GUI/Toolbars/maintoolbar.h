#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QToolBar>
#include <QAction>

class MainToolbar : public QToolBar
{
    void create_actions();
    void create_buttons();
public:
    MainToolbar();
    ~MainToolbar();
    QAction* add_video_act;
    QAction* save_act;
    QAction* open_act;
    QAction* toggle_draw_toolbar_act;

signals:
    void add_video(void);
};

#endif // MAINTOOLBAR_H
