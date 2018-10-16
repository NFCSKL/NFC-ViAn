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
ManipulatorWidget::ManipulatorWidget(int b, double c, double g, QWidget* parent) : QWidget(parent) {
    setWindowTitle("Vian - Color Correction");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    brightness = b;
    contrast = c;
    gamma = g;

    layout = new QFormLayout();
    brightness_layout = new QFormLayout;
    contrast_layout = new QFormLayout;
    gamma_layout = new QFormLayout;
    brightness_slider = new QSlider(Qt::Horizontal, this);
    contrast_slider = new QSlider(Qt::Horizontal, this);
    gamma_slider = new QSlider(Qt::Horizontal, this);
    brightness_value_label = new QLabel(this);
    contrast_value_label = new QLabel(this);
    gamma_value_label = new QLabel(this);
    brightness_value_label->setMinimumWidth(25);
    contrast_value_label->setMinimumWidth(25);
    gamma_value_label->setMinimumWidth(25);

    brightness_slider->setSingleStep(FrameManipulator().BRIGHTNESS_STEP);
    brightness_slider->setMaximum(FrameManipulator().BRIGHTNESS_MAX);
    brightness_slider->setMinimum(FrameManipulator().BRIGHTNESS_MIN);
    brightness_slider->setValue(brightness);
    brightness_value_label->setText(QString::number(brightness_slider->value()));

    contrast_slider->setSingleStep(static_cast<int>(FrameManipulator().CONTRAST_STEP*DOUBLE_TO_INT));
    contrast_slider->setMaximum(static_cast<int>(FrameManipulator().CONTRAST_MAX*DOUBLE_TO_INT));
    contrast_slider->setMinimum(static_cast<int>(FrameManipulator().CONTRAST_MIN*DOUBLE_TO_INT));
    contrast_slider->setValue(static_cast<int>(contrast*DOUBLE_TO_INT));
    contrast_value_label->setText(QString::number(contrast_slider->value()/DOUBLE_TO_INT));

    gamma_slider->setSingleStep(static_cast<int>(FrameManipulator().GAMMA_STEP*DOUBLE_TO_INT));
    gamma_slider->setMaximum(static_cast<int>(FrameManipulator().GAMMA_MAX*DOUBLE_TO_INT));
    gamma_slider->setMinimum(static_cast<int>(FrameManipulator().GAMMA_MIN*DOUBLE_TO_INT));
    gamma_slider->setValue(static_cast<int>(gamma*DOUBLE_TO_INT));
    gamma_value_label->setText(QString::number(gamma_slider->value()/DOUBLE_TO_INT));

    connect(brightness_slider, &QSlider::valueChanged, this, &ManipulatorWidget::b_changed);
    connect(contrast_slider, &QSlider::valueChanged, this, &ManipulatorWidget::c_changed);
    connect(gamma_slider, &QSlider::valueChanged, this, &ManipulatorWidget::g_changed);

    brightness_layout->addRow(brightness_value_label, brightness_slider);
    contrast_layout->addRow(contrast_value_label, contrast_slider);
    gamma_layout->addRow(gamma_value_label, gamma_slider);
    layout->addRow("Brightness", brightness_layout);
    layout->addRow("Contrast", contrast_layout);
    layout->addRow("Gamma", gamma_layout);

    // Button setup
    btn_box = new QDialogButtonBox(Qt::Horizontal, this);

    apply_btn = new QPushButton(tr("&Apply"));

    btn_box->addButton(apply_btn, QDialogButtonBox::AcceptRole);
    btn_box->addButton(QDialogButtonBox::RestoreDefaults)->setText("Default");

    apply_btn->setDisabled(true);
    QString default_tool_tip = "Brightness: "+QString::number(FrameManipulator().BRIGHTNESS_DEFAULT)+"\nContrast: "+QString::number(FrameManipulator().CONTRAST_DEFAULT)+"\nGamma: "+QString::number(FrameManipulator().GAMMA_DEFAULT);
    btn_box->button(QDialogButtonBox::RestoreDefaults)->setToolTip(default_tool_tip);

    connect(apply_btn, &QPushButton::clicked, this, &ManipulatorWidget::ok_clicked);
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
    int b = brightness_slider->value();
    double c = contrast_slider->value()/DOUBLE_TO_INT;
    double g = gamma_slider->value()/DOUBLE_TO_INT;
    apply_btn->setDisabled(true);
    emit values(b, c, g, true);
    emit update_tag(b, c, g);
}

/**
 * @brief ManipulatorWidget::default_clicked
 * Slot function for the default button.
 * Resets the changes to default and saves them in the video state.
 */
void ManipulatorWidget::default_clicked() {
    brightness_slider->setValue(FrameManipulator().BRIGHTNESS_DEFAULT);
    contrast_slider->setValue(static_cast<int>(FrameManipulator().CONTRAST_DEFAULT*DOUBLE_TO_INT));
    gamma_slider->setValue(static_cast<int>(FrameManipulator().GAMMA_DEFAULT*DOUBLE_TO_INT));
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, gamma_slider->value()/DOUBLE_TO_INT, true);
}

/**
 * @brief ManipulatorWidget::b_changed
 * Slot function for when the brightness slider's value is changed
 * Update the frameprocessor with the brightness value
 * @param value
 */
void ManipulatorWidget::b_changed(int value) {
    brightness_value_label->setText(QString::number(value));
    apply_btn->setDisabled(false);
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, gamma_slider->value()/DOUBLE_TO_INT, false);
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
    apply_btn->setDisabled(false);
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, gamma_slider->value()/DOUBLE_TO_INT, false);
}

/**
 * @brief ManipulatorWidget::g_changed
 * Slot function for when the gamma slider's value is changed
 * Update the frameprocessor with the gamma value
 * @param value
 */
void ManipulatorWidget::g_changed(int value) {
    QString text = QString::number(value/DOUBLE_TO_INT);
    gamma_value_label->setText(text);
    apply_btn->setDisabled(false);
    emit values(brightness_slider->value(), contrast_slider->value()/DOUBLE_TO_INT, gamma_slider->value()/DOUBLE_TO_INT, false);
}

/**
 * @brief ManipulatorWidget::set_values
 * Slot function for when the color correction values changes elsewhere.
 * Update the sliders and block unnecessary "value_changed" calls from the sliders.
 * @param b_value
 * @param c_value
 */
void ManipulatorWidget::set_values(int b_value, double c_value, double g_value) {
    blockSignals(true);
    brightness_slider->setValue(b_value);
    contrast_slider->setValue(static_cast<int>(c_value*DOUBLE_TO_INT));
    gamma_slider->setValue(static_cast<int>(g_value*DOUBLE_TO_INT));
    blockSignals(false);
    apply_btn->setDisabled(true);
}
