#include "statusbar.h"
#include <QDebug>

StatusBar::StatusBar() {
    setCursor(Qt::WaitCursor);
}

void StatusBar::on_set_status_bar(std::string status, int timer) {
    showMessage(QString::fromStdString(status), timer);
}
