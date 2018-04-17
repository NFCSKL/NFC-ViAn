#include "textdialog.h"
#include <QBoxLayout>
#include <QPushButton>

TextDialog::TextDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Vian - New Text");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout* vertical_layout = new QVBoxLayout;
    name = new QLineEdit(this);
    double_box = new QDoubleSpinBox;
    double_box->setMaximum(Text::FONT_SCALE_MAX);
    double_box->setMinimum(Text::FONT_SCALE_MIN);
    double_box->setDecimals(Text::FONT_SCALE_DECIMALS);
    double_box->setSingleStep(Text::FONT_SCALE_STEP);
    double_box->setValue(1);

    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    QFormLayout* text_name_layout = new QFormLayout;
    text_name_layout->addRow("Enter Text: ", name);
    QFormLayout* thickness_layout = new QFormLayout;
    thickness_layout->addRow("Thickness:", double_box);

    vertical_layout->addLayout(text_name_layout);
    vertical_layout->addLayout(thickness_layout);
    vertical_layout->addWidget(btn_box);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &TextDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &TextDialog::cancel_btn_clicked);
    setLayout(vertical_layout);
}

/**
 * @brief TextDialog::ok_btn_clicked
 * Send text name and close widget
 */
void TextDialog::ok_btn_clicked() {
    if (name->text() == "") {
        emit text("Text", double_box->value());
    } else {
        emit text(name->text(), double_box->value());
    }
    close();
}

/**
 * @brief TextDialog::cancel_btn_clicked
 * Close widget
 */
void TextDialog::cancel_btn_clicked() {
    close();
}
