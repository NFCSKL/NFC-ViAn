#include "statusbar.h"
#include <QDebug>

StatusBar::StatusBar() {

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
