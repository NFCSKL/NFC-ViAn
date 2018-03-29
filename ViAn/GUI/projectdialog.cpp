#include "projectdialog.h"
#include <QSize>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

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
    path_text = new QLineEdit(default_path, this);
    name_text = new QLineEdit(this);
    path_text = new QLineEdit(this);
    QPushButton* browse_btn = new QPushButton(tr("Browse"), this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);

    setTabOrder(name_text, path_text);
    setTabOrder(path_text, browse_btn);

    browse_btn->setFixedWidth(80);
    browse_btn->setFixedHeight(path_text->height());
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    path_text->setText("C:/");

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
}

void ProjectDialog::browse_btn_clicked() {
    QDir standard;
    standard.mkpath(default_path);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose project path"),
                                                    default_path);
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
//    emit project_path(name_text->text(), path_text->text());
    *m_name = name_text->text();
    *m_path = path_text->text() + "/";
    accept();
}

void ProjectDialog::cancel_btn_clicked() {
    reject();
}

