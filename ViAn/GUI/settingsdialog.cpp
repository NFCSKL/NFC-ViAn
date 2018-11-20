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
    setMinimumSize(500,100);

    layout = new QFormLayout();
    add_widgets();

    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    // Connects
    connect(thickness_slider, &QSlider::valueChanged, this, &SettingsDialog::thickness_changed);
    connect(page_step_slider, &QSlider::valueChanged, this, &SettingsDialog::page_step_changed);
    connect(widget_max_slider, &QSlider::valueChanged, this, &SettingsDialog::widget_max_changed);
    connect(thumbnail_size_slider, &QSlider::valueChanged, this, &SettingsDialog::thumbnail_size_changed);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SettingsDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &SettingsDialog::restore_btn_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);
    show();
}

void SettingsDialog::add_widgets() {
    Singleton* s = Singleton::get_instance();
    // Add thickness
    thickness_layout = new QFormLayout();
    // Add slider
    thickness_slider = new QSlider(Qt::Horizontal, this);
    thickness_slider->setMaximum(Constants::LINE_THICKNESS_MAX);
    thickness_slider->setMinimum(Constants::LINE_THICKNESS_MIN);
    thickness_slider->setValue(s->LINE_THICKNESS);
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
    page_step_slider->setMaximum(Constants::PAGE_STEP_MAX);
    page_step_slider->setMinimum(Constants::PAGE_STEP_MIN);
    page_step_slider->setValue(s->PAGE_STEP);
    // Add label
    page_step_label = new QLabel(this);
    page_step_label->setMinimumWidth(25);
    page_step_label->setText(QString::number(page_step_slider->value()));

    page_step_layout->addRow(page_step_label, page_step_slider);
    layout->addRow("Page step", page_step_layout);

    // Add widget max
    widget_max_layout = new QFormLayout();
    // Add slider
    widget_max_slider = new QSlider(Qt::Horizontal, this);
    widget_max_slider->setMaximum(Constants::FLOATING_WIDGET_LIMIT_MAX);
    widget_max_slider->setMinimum(Constants::FLOATING_WIDGET_LIMIT_MIN);
    widget_max_slider->setValue(s->FLOATING_WIDGET_MAX);
    // Add label
    widget_max_label = new QLabel(this);
    widget_max_label->setMinimumWidth(25);
    widget_max_label->setText(QString::number(widget_max_slider->value()));

    widget_max_layout->addRow(widget_max_label, widget_max_slider);
    layout->addRow("Max number of floating widgets", widget_max_layout);

    // Add widget max
    thumbnail_size_layout = new QFormLayout();
    // Add slider
    thumbnail_size_slider = new QSlider(Qt::Horizontal, this);
    thumbnail_size_slider->setMaximum(Constants::PROJ_THUMBNAIL_MAX);
    thumbnail_size_slider->setMinimum(Constants::PROJ_THUMBNAIL_MIN);
    thumbnail_size_slider->setValue(s->PROJ_THUMBNAIL_SIZE.width());
    // Add label
    thumbnail_size_label = new QLabel(this);
    thumbnail_size_label->setMinimumWidth(25);
    thumbnail_size_label->setText(QString::number(thumbnail_size_slider->value()));

    thumbnail_size_layout->addRow(thumbnail_size_label, thumbnail_size_slider);
    layout->addRow("Video thumbnail size", thumbnail_size_layout);

}

void SettingsDialog::thickness_changed(int value) {
    thickness_label->setText(QString::number(value));
}

void SettingsDialog::page_step_changed(int value) {
    page_step_label->setText(QString::number(value));
}

void SettingsDialog::widget_max_changed(int value) {
    widget_max_label->setText(QString::number(value));
}

void SettingsDialog::thumbnail_size_changed(int value) {
    thumbnail_size_label->setText(QString::number(value));
}

void SettingsDialog::ok_btn_clicked() {
    Singleton* s = Singleton::get_instance();
    s->LINE_THICKNESS = thickness_slider->value();
    s->PAGE_STEP = page_step_slider->value(); // TODO update slider
    s->FLOATING_WIDGET_MAX = widget_max_slider->value();
    s->PROJ_THUMBNAIL_SIZE = QSize(thumbnail_size_slider->value(),thumbnail_size_slider->value());
    accept();
}

void SettingsDialog::restore_btn_clicked() {
    // Reset;
}
