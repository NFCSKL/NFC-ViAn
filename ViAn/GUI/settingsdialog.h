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
    QFormLayout* page_step_layout;
    QFormLayout* widget_max_layout;

    QSlider* thickness_slider;
    QSlider* page_step_slider;
    QSlider* widget_max_slider;

    QLabel* thickness_label;
    QLabel* page_step_label;
    QLabel* widget_max_label;

    QDialogButtonBox* btn_box;
public:
    SettingsDialog(QWidget* parent = nullptr);

private slots:
    void thickness_changed(int value);
    void page_step_changed(int value);
    void widget_max_changed(int value);
    void ok_btn_clicked();
    void restore_btn_clicked();
};

#endif // SETTINGSDIALOG_H
