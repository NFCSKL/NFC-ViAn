#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QFormLayout;
class QLabel;
class QSlider;
class SettingsDialog : public QDialog
{
    Q_OBJECT

    QFormLayout* layout;
    QFormLayout* thickness_layout;

    QLabel* thickness_label;
    QSlider* thickness_slider;

    QDialogButtonBox* btn_box;
public:
    SettingsDialog(QWidget* parent = nullptr);
};

#endif // SETTINGSDIALOG_H
