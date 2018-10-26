#include "statusbar.h"

#include <QDebug>
#include <QProgressBar>

StatusBar::StatusBar() {
    ana_prog = new QProgressBar();
    ana_prog->setOrientation(Qt::Horizontal);
    ana_prog->setMaximumHeight(PROG_BAR_HEIGHT);
    ana_prog->setMaximumWidth(PROG_BAR_WIDTH);
    addPermanentWidget(ana_prog);
    remove_analysis_bar();
}

/**
 * @brief StatusBar::on_set_status_bar
 * @param status String to be shown on the status bar
 * @param timer Duration of the status bar message
 * Set status to the status bar for timer duration.
 */
void StatusBar::on_set_status_bar(QString status, int timer) {
    showMessage(status, timer);
}

void StatusBar::add_analysis_bar() {
    ana_prog->show();
}

void StatusBar::update_analysis_bar(int progress) {
    ana_prog->setValue(progress);
}

void StatusBar::remove_analysis_bar() {
    ana_prog->hide();
}
