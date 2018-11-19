#include "settingsdialog.h"

#include "constants.h"

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

    Singleton* s = Singleton::get_instance();

    layout = new QFormLayout();

    // Add thickness
    thickness_layout = new QFormLayout();
    // Add slider
    thickness_slider = new QSlider(Qt::Horizontal, this);
    thickness_slider->setValue(s->LINE_THICKNESS);
    thickness_slider->setMaximum(Constants::LINE_THICKNESS_MAX);
    thickness_slider->setMinimum(Constants::LINE_THICKNESS_MIN);
    // Add label
    thickness_label = new QLabel(this);
    thickness_label->setMinimumWidth(25);
    thickness_label->setText(QString::number(thickness_slider->value()));

    thickness_layout->addRow(thickness_label, thickness_slider);
    layout->addRow("Thickness", thickness_layout);

    // Add page step
    page_step_layout = new QFormLayout();
    // Add slider
    page_step_slider = new QSlider(Qt::Horizontal, this);
    page_step_slider->setValue(s->PAGE_STEP);
    page_step_slider->setMaximum(Constants::PAGE_STEP_MAX);
    page_step_slider->setMinimum(Constants::PAGE_STEP_MIN);
    // Add label
    page_step_label = new QLabel(this);
    page_step_label->setMinimumWidth(25);
    page_step_label->setText(QString::number(page_step_slider->value()));

    page_step_layout->addRow(page_step_label, page_step_slider);
    layout->addRow("Page step", page_step_layout);

    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    // Connects
    connect(thickness_slider, &QSlider::valueChanged, this, &SettingsDialog::thickness_changed);
    connect(page_step_slider, &QSlider::valueChanged, this, &SettingsDialog::page_step_changed);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SettingsDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &SettingsDialog::restore_btn_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);
    show();
}

void SettingsDialog::thickness_changed(int value) {
    thickness_label->setText(QString::number(value));
}

void SettingsDialog::page_step_changed(int value) {
    page_step_label->setText(QString::number(value));
}

void SettingsDialog::ok_btn_clicked() {
    Singleton* s = Singleton::get_instance();
    s->LINE_THICKNESS = thickness_slider->value();
    s->PAGE_STEP = page_step_slider->value(); // TODO update slider
    close();
}

void SettingsDialog::restore_btn_clicked() {
    close();
}
