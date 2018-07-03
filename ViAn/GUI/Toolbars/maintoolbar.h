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
    QAction* save_act;
    QAction* open_act;
    QAction* add_video_act;
    QAction* open_folder_act;

signals:
};

#endif // MAINTOOLBAR_H
