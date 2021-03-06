#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QToolBar>

class QAction;

class MainToolbar : public QToolBar
{
    void create_actions();
    void create_buttons();
public:
    MainToolbar();
    QAction* save_act;
    QAction* open_act;
    QAction* open_recent_act;
    QAction* add_video_act;
    QAction* add_img_seq_act;
    QAction* open_folder_act;
};

#endif // MAINTOOLBAR_H
