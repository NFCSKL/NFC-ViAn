#include "manipulatordialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <Video/framemanipulator.h>

ManipulatorDialog::ManipulatorDialog(QWidget* parent) : QDialog(parent) {
    QFormLayout* layout = new QFormLayout;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Spin box setup
    brightness_box = new QSpinBox(this);
    contrast_box = new QDoubleSpinBox(this);
    brightness_box->setSingleStep(FrameManipulator().BRIGHTNESS_STEP);
    brightness_box->setMinimum(FrameManipulator().BRIGHTNESS_MIN);
    brightness_box->setMaximum(FrameManipulator().BRIGHTNESS_MAX);
    contrast_box->setSingleStep(FrameManipulator().CONTRAST_STEP);
    contrast_box->setMinimum(FrameManipulator().CONTRAST_MIN);
    contrast_box->setMaximum(FrameManipulator().CONTRAST_MAX);

    connect(this->brightness_box, SIGNAL(valueChanged(int)), this, SLOT(b_changed(int)));
    connect(this->contrast_box, SIGNAL(valueChanged(double)), this, SLOT(c_changed(double)));

    layout->addRow("Brightness", brightness_box);
    layout->addRow("Contrast", contrast_box);

    //Button setup
    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Apply)->setEnabled(false);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ManipulatorDialog::ok_clicked);
    connect(btn_box->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &ManipulatorDialog::apply_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ManipulatorDialog::cancel_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &ManipulatorDialog::reset_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);
}

/**
 * @brief ManipulatorDialog::ok_clicked
 * Emits the current values and closes the dialog
 */
void ManipulatorDialog::ok_clicked() {
    emit values(brightness_box->value(), contrast_box->value());
    close();
}

/**
 * @brief ManipulatorDialog::apply_clicked
 * Emits the current values and disables the apply button
 */
void ManipulatorDialog::apply_clicked() {
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(false);
    emit values(brightness_box->value(), contrast_box->value());
}

/**
 * @brief ManipulatorDialog::cancel_clicked
 * Closes the dialog
 */
void ManipulatorDialog::cancel_clicked() {
    close();
}

/**
 * @brief ManipulatorDialog::reset_clicked
 * Restores the spin box values to those defined in frame manipulator
 */
void ManipulatorDialog::reset_clicked() {
    brightness_box->setValue(FrameManipulator().BRIGHTNESS_DEFAULT);
    contrast_box->setValue(FrameManipulator().CONTRAST_DEFAULT);
}

/**
 * @brief ManipulatorDialog::b_changed
 * Enables the apply button
 * @param value
 */
void ManipulatorDialog::b_changed(int value) {
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(true);
}

/**
 * @brief ManipulatorDialog::c_changed
 * Enables the apply button
 * @param value
 */
void ManipulatorDialog::c_changed(double value) {
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(true);
}
