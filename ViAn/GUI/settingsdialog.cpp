#include "settingsdialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));
    setWindowTitle("Vian - Settings");
    setWindowIcon(QIcon("../ViAn/Icons/settings.png"));
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300,100);

    layout = new QFormLayout();

    // Add thickness
    thickness_layout = new QFormLayout();
    thickness_slider = new QSlider(Qt::Horizontal, this);
    thickness_label = new QLabel(this);
    thickness_label->setMinimumWidth(25);
    thickness_label->setText(QString::number(thickness_slider->value()));
    thickness_layout->addRow(thickness_label, thickness_slider);

    layout->addRow("Thickness", thickness_layout);

    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    // Connects

    layout->addWidget(btn_box);
    setLayout(layout);
    show();
}
