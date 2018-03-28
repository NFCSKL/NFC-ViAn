#include "maintoolbar.h"
#include <QIcon>

/**
 * @brief MainToolbar::MainToolbar
 */
MainToolbar::MainToolbar() {
    create_actions();
    create_buttons();
}

MainToolbar::~MainToolbar() {
    delete add_video_act;
    delete save_act;
    delete open_act;
    delete toggle_draw_toolbar_act;
}

/**
 * @brief MainToolbar::create_actions
 * Creates all actions and sets the drawing toolbar action to be checkable
 */
void MainToolbar::create_actions() {
    add_video_act = new QAction(QIcon("../ViAn/Icons/add_video.png"), tr("Add video"), this);
    save_act = new QAction(QIcon("../ViAn/Icons/save.png"),tr("Save project"), this);
    open_act = new QAction(QIcon("../ViAn/Icons/open.png"), tr("Load project"), this);
    toggle_draw_toolbar_act = new QAction(QIcon("../ViAn/Icons/toggle_draw.png"), tr("Toggle drawing toolbar"), this);
    toggle_draw_toolbar_act->setCheckable(true);
    toggle_draw_toolbar_act->setChecked(true);
}

/**
 * @brief MainToolbar::create_buttons
 * Adds all actions to the toolbar
 */
void MainToolbar::create_buttons() {
    addAction(save_act);
    addAction(open_act);
    addAction(add_video_act);
    addAction(toggle_draw_toolbar_act);
}
