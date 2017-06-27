#include "manipulatordialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QDebug>
#include <Video/framemanipulator.h>

ManipulatorDialog::ManipulatorDialog(QWidget* parent) : QDialog(parent) {
    qDebug("here");
    QFormLayout* layout = new QFormLayout;

    brightness_box = new QSpinBox(this);
    contrast_box = new QDoubleSpinBox(this);

    brightness_box->setSingleStep(FrameManipulator().BRIGHTNESS_STEP);
    brightness_box->setMinimum(FrameManipulator().BRIGHTNESS_MIN);
    brightness_box->setMaximum(FrameManipulator().BRIGHTNESS_MAX);
    contrast_box->setSingleStep(FrameManipulator().CONTRAST_STEP);
    contrast_box->setMinimum(FrameManipulator().CONTRAST_MIN);
    contrast_box->setMaximum(FrameManipulator().CONTRAST_MAX);

    btn_box = new QDialogButtonBox(Qt::Horizontal, this);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Apply);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(false);

    layout->addRow("Brightness", brightness_box);
    layout->addRow("Contrast", contrast_box);
    layout->addWidget(btn_box);
    setLayout(layout);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ManipulatorDialog::ok_clicked);
    connect(btn_box->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &ManipulatorDialog::apply_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ManipulatorDialog::cancel_clicked);
    connect(this->brightness_box, SIGNAL(valueChanged(int)), this, SLOT(b_changed(int)));
    connect(this->contrast_box, SIGNAL(valueChanged(double)), this, SLOT(c_changed(double)));
}

void ManipulatorDialog::ok_clicked() {
    emit values(brightness_box->value(), contrast_box->value());
    close();
}

void ManipulatorDialog::apply_clicked() {
    qDebug("apply");
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(false);
    emit values(brightness_box->value(), contrast_box->value());
}

void ManipulatorDialog::cancel_clicked() {
    close();
}

void ManipulatorDialog::b_changed(int value) {
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void ManipulatorDialog::c_changed(double value) {
    btn_box->button(QDialogButtonBox::Apply)->setEnabled(true);
}
