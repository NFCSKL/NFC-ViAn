#include "tagdialog.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

/**
 * @brief TagDialog::TagDialog
 * @param parent
 * Setup tag widget
 */
TagDialog::TagDialog(QString string, QWidget *parent) : QDialog(parent) {
    // Set title
    setWindowTitle("Vian - New " + string);
    // Remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Setup widget layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    vertical_layout->deleteLater();
    name = new QLineEdit(this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    QFormLayout* tag_name_layout = new QFormLayout;
    tag_name_layout->deleteLater();
    tag_name_layout->addRow(string + " name: ", name);
    vertical_layout->addLayout(tag_name_layout);
    vertical_layout->addWidget(btn_box);

    // Connect buttons
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &TagDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &TagDialog::cancel_btn_clicked);
    setLayout(vertical_layout);
}

TagDialog::~TagDialog() {
    delete name;
    delete btn_box;
}

/**
 * @brief TagDialog::ok_btn_clicked
 * Send tag name and close widget
 */
void TagDialog::ok_btn_clicked() {
    emit tag_name(name->text());
    close();
}

/**
 * @brief TagDialog::cancel_btn_clicked
 * Close widget
 */
void TagDialog::cancel_btn_clicked() {    
    close();
}
