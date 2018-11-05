#include "maintoolbar.h"

#include <QAction>
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
    open_act = new QAction(QIcon("../ViAn/Icons/open.png"), tr("Open project"), this);
    open_recent_act = new QAction(QIcon("../ViAn/Icons/home.png"), tr("Open recent projects dialog"), this);
    add_video_act = new QAction(QIcon("../ViAn/Icons/add_video.png"), tr("Import video"), this);
    add_img_seq_act = new QAction(QIcon("../ViAn/Icons/image_sequence.png"), tr("Import images"), this);
    open_folder_act = new QAction(QIcon("../ViAn/Icons/new_folder.png"), tr("Open project folder"), this);
}

/**
 * @brief MainToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void MainToolbar::create_buttons() {
    addAction(save_act);
    addAction(open_act);
    addAction(open_recent_act);
    addAction(add_video_act);
    addAction(add_img_seq_act);
    addAction(open_folder_act);
}
