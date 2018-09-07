#include "manipulatorwidget.h"
#include <QPushButton>
#include <QDebug>

/**
 * @brief ManipulatorWidget::ManipulatorWidget
 * Widget for the color corrections.
 * Currently contains settigns for contrast and brightness.
 * @param b - Brightness
 * @param c - Contrast
 * @param parent
 */
ManipulatorWidget::ManipulatorWidget(int b, double c, QWidget* parent) : QWidget(parent) {
    setWindowTitle("Vian - Color Correction");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    brightness = b;
    contrast = c;

    layout = new QFormLayout();
    brightness_layout = new QFormLayout;
    contrast_layout = new QFormLayout;
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

    connect(brightness_slider, &QSlider::valueChanged, this, &ManipulatorWidget::b_changed);
    connect(contrast_slider, &QSlider::valueChanged, this, &ManipulatorWidget::c_changed);

    brightness_layout->addRow(brightness_value_label, brightness_slider);
    contrast_layout->addRow(contrast_value_label, contrast_slider);
    layout->addRow("Brightness", brightness_layout);
    layout->addRow("Contrast", contrast_layout);

    // Button setup
    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok)->setText("Apply");
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ManipulatorWidget::ok_clicked);
    connect(btn_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &ManipulatorWidget::default_clicked);

    layout->addWidget(btn_box);
    setLayout(layout);

    show();
}

/**
 * @brief ManipulatorWidget::ok_clicked
 * Slot function for the ok ("Apply") button
 * Saves the changes to the video state
 */
void ManipulatorWidget::ok_clicked() {
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, true);
}

/**
 * @brief ManipulatorWidget::default_clicked
 * Slot function for the default button.
 * Resets the changes to default and saves them in the video state.
 */
void ManipulatorWidget::default_clicked() {
    brightness_slider->setValue(FrameManipulator().BRIGHTNESS_DEFAULT);
    contrast_slider->setValue(FrameManipulator().CONTRAST_DEFAULT*DOUBLE_TO_INT);
    ok_clicked();
}

/**
 * @brief ManipulatorWidget::b_changed
 * Slot function for when the brightness slider's value is changed
 * Update the frameprocessor with the brightness value
 * @param value
 */
void ManipulatorWidget::b_changed(int value) {
    brightness_value_label->setText(QString::number(value));
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, false);
}

/**
 * @brief ManipulatorWidget::c_changed
 * Slot function for when the contrast slider's value is changed
 * Update the frameprocessor with the contrast value
 * @param value
 */
void ManipulatorWidget::c_changed(int value) {
    QString text = QString::number(value/DOUBLE_TO_INT);
    contrast_value_label->setText(text);
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, false);
}

/**
 * @brief ManipulatorWidget::set_values
 * Slot function for when the color correction values changes elsewhere.
 * Update the sliders and block unnecessary "value_changed" calls from the sliders.
 * @param b_value
 * @param c_value
 */
void ManipulatorWidget::set_values(int b_value, double c_value) {
    blockSignals(true);
    brightness_slider->setValue(b_value);
    contrast_slider->setValue(c_value*DOUBLE_TO_INT);
    blockSignals(false);
}
