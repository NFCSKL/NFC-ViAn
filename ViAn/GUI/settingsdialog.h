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
    QFormLayout* thumbnail_size_layout;
    QFormLayout* class_file_layout;
    QFormLayout* config_file_layout;
    QFormLayout* weight_file_layout;

    QSlider* thickness_slider;
    QSlider* page_step_slider;
    QSlider* widget_max_slider;
    QSlider* thumbnail_size_slider;

    QLabel* class_file_path;
    QLabel* config_file_path;
    QLabel* weight_file_path;

    QPushButton* class_file_btn;
    QPushButton* config_file_btn;
    QPushButton* weight_file_btn;

    QLabel* thickness_label;
    QLabel* page_step_label;
    QLabel* widget_max_label;
    QLabel* thumbnail_size_label;
    QLabel* class_file_label;
    QLabel* config_file_label;
    QLabel* weight_file_label;

    QDialogButtonBox* btn_box;

    void add_widgets();
public:
    SettingsDialog(QWidget* parent = nullptr);

private slots:
    void thickness_changed(int value);
    void page_step_changed(int value);
    void widget_max_changed(int value);
    void thumbnail_size_changed(int value);
    void class_btn_clicked();
    void config_btn_clicked();
    void weight_btn_clicked();
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // SETTINGSDIALOG_H
