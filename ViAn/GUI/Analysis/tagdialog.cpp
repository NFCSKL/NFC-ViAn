#include "tagdialog.h"
#include <QBoxLayout>
#include <QPushButton>

/**
 * @brief TagDialog::TagDialog
 * @param parent
 * Setup tag widget
 */
TagDialog::TagDialog(QWidget *parent) : QDialog(parent) {
    // Set title
    setWindowTitle("Vian - New Tag");
    // Remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Setup widget layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    name = new QLineEdit(this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    QFormLayout* tag_name_layout = new QFormLayout;
    tag_name_layout->addRow("Tag name: ", name);
    vertical_layout->addLayout(tag_name_layout);
    vertical_layout->addWidget(btn_box);

    // Connect buttons
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &TagDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &TagDialog::cancel_btn_clicked);
    setLayout(vertical_layout);
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
