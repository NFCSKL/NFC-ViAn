#include "pathdialog.h"

#include "utility.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

PathDialog::PathDialog(std::string* path, QWidget* parent, QString default_path) : QDialog(parent) {
    setWindowTitle("ViAn - Update video path");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(MIN_WIDTH);

    m_path = path;
    QString q_path = QString::fromStdString(*path);
    m_dir = q_path.left(q_path.lastIndexOf('/') + 1);
    m_name = QString::fromStdString(Utility::name_from_path(*path));
    m_default_path = default_path;

    QVBoxLayout* vertical_layout = new QVBoxLayout;
    QHBoxLayout* browse_layout = new QHBoxLayout;

    path_text = new QLineEdit(m_dir, this);
    QPushButton* browse_btn = new QPushButton(tr("Browse"), this);

    browse_layout->addWidget(path_text);
    browse_layout->addWidget(browse_btn);

    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    //enable_ok_btn(false);

    vertical_layout->addLayout(browse_layout);
    vertical_layout->addWidget(btn_box);
    setLayout(vertical_layout);

    connect(browse_btn, &QPushButton::clicked, this, &PathDialog::browse_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &PathDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &PathDialog::cancel_btn_clicked);
}


void PathDialog::enable_ok_btn(const bool& enable) {
    btn_box->button(QDialogButtonBox::Ok)->setEnabled(enable);
}

void PathDialog::browse_btn_clicked() {
    QDir standard;
    standard.mkpath(m_default_path);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose project path"),
                                                    m_default_path);
    if(!dir.isEmpty()) {
        path_text->setText(dir);
    }
}

void PathDialog::ok_btn_clicked() {
    *m_path = (path_text->text() + "/" + m_name).toStdString();
    accept();
}

void PathDialog::cancel_btn_clicked() {
    reject();
}

