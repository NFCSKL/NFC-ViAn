#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QFormLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

class SettingsDialog : public QDialog {
    Q_OBJECT

    QFormLayout* layout;
    QFormLayout* thickness_layout;
    //QFormLayout* floating_widget_layout;
    QLabel* thickness_label;
    QLabel* floating_widget_label;
    QSlider* thickness_slider;
    QLineEdit* floating_widget_edit;
    QDialogButtonBox* btn_box;

public:
    SettingsDialog(QWidget *parent = nullptr);

private slots:
    void ok_clicked();
    void default_clicked();
};

#endif // SETTINGSDIALOG_H
