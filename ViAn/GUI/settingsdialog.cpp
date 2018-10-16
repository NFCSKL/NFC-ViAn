#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));
    setWindowTitle("ViAn - Settings");
    setWindowIcon(QIcon("../ViAn/Icons/settings.png"));
    setAttribute(Qt::WA_DeleteOnClose);

    layout = new QFormLayout();

    // Add thickness
    thickness_layout = new QFormLayout();
    thickness_slider = new QSlider(Qt::Horizontal, this);
    thickness_label = new QLabel(this);
    thickness_label->setMinimumWidth(25);
    thickness_label->setText(QString::number(thickness_slider->value()));
    thickness_layout->addRow(thickness_label, thickness_slider);

    layout->addRow("Thickness", thickness_layout);

    floating_widget_edit = new QLineEdit();
    //floating_widget_edit->setText()



    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SettingsDialog::ok_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &SettingsDialog::default_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);

    show();
}

void SettingsDialog::ok_clicked() {

}

void SettingsDialog::default_clicked() {

}
