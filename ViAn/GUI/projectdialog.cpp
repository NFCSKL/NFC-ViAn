#include "projectdialog.h"
#include <QSize>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

/**
 * @brief ProjectDialog::disable_ok_btn
 * Disables the ok button in the button box
 */
void ProjectDialog::enable_ok_btn(const bool& enable) {
    btn_box->button(QDialogButtonBox::Ok)->setEnabled(enable);
}

/**
 * @brief ProjectDialog::ProjectDialog
 * @param parent
 * Dialog usd to create new projects.
 */
ProjectDialog::ProjectDialog(QString* name, QString* path, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Save project as");
    setModal(true);
    m_name = name;
    m_path = path;

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    path_text = new QLineEdit(DEFAULT_PATH, this);
    name_text = new QLineEdit(this);
    QPushButton* browse_btn = new QPushButton(tr("Browse"), this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);

    setTabOrder(name_text, path_text);
    setTabOrder(path_text, browse_btn);

    browse_btn->setFixedWidth(80);
    browse_btn->setFixedHeight(path_text->height());
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    enable_ok_btn(false);


    QHBoxLayout* browse_layout = new QHBoxLayout;
    browse_layout->addWidget(path_text);
    browse_layout->addWidget(browse_btn);

    QFormLayout* text_btn_layout = new QFormLayout;
    text_btn_layout->addRow("Name:", name_text);
    text_btn_layout->addRow("Path:", browse_layout);

    vertical_layout->addLayout(text_btn_layout);
    vertical_layout->addWidget(btn_box);
    setLayout(vertical_layout);

    connect(browse_btn, &QPushButton::clicked, this, &ProjectDialog::browse_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ProjectDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ProjectDialog::cancel_btn_clicked);

    connect(name_text, &QLineEdit::textEdited, this, &ProjectDialog::on_name_text_edited);

}

void ProjectDialog::browse_btn_clicked() {
    QDir standard;
    standard.mkpath(DEFAULT_PATH);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose project path"),
                                                    DEFAULT_PATH);
    if(!dir.isEmpty()) {
        path_text->setText(dir);
    }
}

void ProjectDialog::ok_btn_clicked() {
    QString path = path_text->text() + "/" + name_text->text() + "/" + name_text->text() + ".vian";

    QFile pathFile(path);
    if (pathFile.exists()) {
        // Create confirmation dialog since the path already exists
        QMessageBox msg_box;
        msg_box.setModal(true);
        msg_box.setText("This project already exist. Are you sure you want to continue?");
        msg_box.setInformativeText("Open will open the existing project.");
        msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Open);
        msg_box.setDefaultButton(QMessageBox::No);
        int reply = msg_box.exec();
        // Open the already existing project
        if (reply == QMessageBox::Open) {
            emit open_project(path);
            close();
            return;
        }
        if (reply != QMessageBox::Yes) return;
    }
    *m_name = name_text->text();
    *m_path = path_text->text() + "/";
    accept();
}

void ProjectDialog::cancel_btn_clicked() {
    reject();
}

/**
 * @brief ProjectDialog::on_name_text_edited
 * Slot function used to verify the name text content
 * @param new_text
 */
void ProjectDialog::on_name_text_edited(const QString &new_text) {
    enable_ok_btn(!new_text.isEmpty());
}

