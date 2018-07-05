#include "manipulatordialog.h"
#include <QPushButton>
#include <Video/framemanipulator.h>

ManipulatorDialog::ManipulatorDialog(int b, double c, QWidget* parent) : QDialog(parent) {
    contrast = c;
    brightness = b;

    layout = new QFormLayout;
    brightness_layout = new QFormLayout;
    contrast_layout = new QFormLayout;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    brightness_slider = new QSlider(Qt::Horizontal, this);
    contrast_slider = new QSlider(Qt::Horizontal, this);
    brightness_value_label = new QLabel(this);
    contrast_value_label = new QLabel(this);
    brightness_value_label->setMinimumWidth(25);
    contrast_value_label->setMinimumWidth(25);
    brightness_slider->setSingleStep(FrameManipulator().BRIGHTNESS_STEP);
    brightness_slider->setMaximum(FrameManipulator().BRIGHTNESS_MAX);
    brightness_slider->setMinimum(FrameManipulator().BRIGHTNESS_MIN);
    brightness_slider->setValue(brightness);
    brightness_value_label->setText(QString::number(brightness_slider->value()));

    contrast_slider->setSingleStep(FrameManipulator().CONTRAST_STEP*DOUBLE_TO_INT);
    contrast_slider->setMaximum(FrameManipulator().CONTRAST_MAX*DOUBLE_TO_INT);
    contrast_slider->setMinimum(FrameManipulator().CONTRAST_MIN*DOUBLE_TO_INT);
    contrast_slider->setValue(contrast*DOUBLE_TO_INT);
    contrast_value_label->setText(QString::number(contrast_slider->value()/DOUBLE_TO_INT));

    connect(this->brightness_slider, SIGNAL(valueChanged(int)), this, SLOT(b_changed(int)));
    connect(this->contrast_slider, SIGNAL(valueChanged(int)), this, SLOT(c_changed(int)));

    brightness_layout->addRow(brightness_value_label, brightness_slider);
    contrast_layout->addRow(contrast_value_label, contrast_slider);
    layout->addRow("Brightness", brightness_layout);
    layout->addRow("Contrast", contrast_layout);

    //Button setup
    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ManipulatorDialog::ok_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ManipulatorDialog::cancel_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &ManipulatorDialog::reset_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);
    setWindowTitle(tr("Brightness and Contrast"));
    setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * @brief ManipulatorDialog::ok_clicked
 * Emits the current values and closes the dialog
 */
void ManipulatorDialog::ok_clicked() {
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT);
    close();
}

/**
 * @brief ManipulatorDialog::cancel_clicked
 * Closes the dialog
 */
void ManipulatorDialog::cancel_clicked() {
    emit values(brightness, contrast);
    close();
}

/**
 * @brief ManipulatorDialog::reset_clicked
 * Restores the spin box values to those defined in frame manipulator
 */
void ManipulatorDialog::reset_clicked() {
    brightness_slider->setValue(FrameManipulator().BRIGHTNESS_DEFAULT);
    contrast_slider->setValue(FrameManipulator().CONTRAST_DEFAULT*DOUBLE_TO_INT);
}

/**
 * @brief ManipulatorDialog::b_changed
 * Enables the apply button
 * @param value
 */
void ManipulatorDialog::b_changed(int value) {
    brightness_value_label->setText(QString::number(value));
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT);
}

/**
 * @brief ManipulatorDialog::c_changed
 * Enables the apply button
 * @param value
 */
void ManipulatorDialog::c_changed(int value) {
    QString text = QString::number(value/DOUBLE_TO_INT);
    contrast_value_label->setText(text);
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT);
}
