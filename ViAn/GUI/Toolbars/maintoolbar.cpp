#include "maintoolbar.h"
#include <QIcon>

/**
 * @brief MainToolbar::MainToolbar
 */
MainToolbar::MainToolbar() {
    create_actions();
    create_buttons();
}

/**
 * @brief MainToolbar::create_actions
 * Creates all actions and sets the drawing toolbar action to be checkable
 */
void MainToolbar::create_actions() {
    save_act = new QAction(QIcon("../ViAn/Icons/save.png"),tr("Save project"), this);
    open_act = new QAction(QIcon("../ViAn/Icons/open.png"), tr("Load project"), this);
    add_video_act = new QAction(QIcon("../ViAn/Icons/add_video.png"), tr("Add video"), this);
    open_folder_act = new QAction(QIcon("../ViAn/Icons/computer.png"), tr("Open folder"), this);
}

/**
 * @brief MainToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void MainToolbar::create_buttons() {
    addAction(save_act);
    addAction(open_act);
    addAction(add_video_act);
    addAction(open_folder_act);
}
