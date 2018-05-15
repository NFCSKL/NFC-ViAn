#ifndef VIDEOINFODIALOG_H
#define VIDEOINFODIALOG_H

#include <QObject>
#include <QDialog>

class VideoInfoDialog : public QDialog {
    Q_OBJECT
public:
    VideoInfoDialog(QWidget* parent = nullptr);
};

#endif // VIDEOINFODIALOG_H
