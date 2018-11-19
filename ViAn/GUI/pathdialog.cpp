#include "pathdialog.h"

#include "constants.h"
#include "utility.h"
#include "viewpathdialog.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

PathDialog::PathDialog(std::string* path, QWidget* parent, QString default_path) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(Constants::DIALOG_MIN_WIDTH);

    m_path = path;
    m_q_path = QString::fromStdString(*path);
    m_dir = m_q_path.left(m_q_path.lastIndexOf('/') + 1);
    m_name = QString::fromStdString(Utility::name_from_path(*path));
    m_default_path = default_path;

    setWindowTitle("Where is file " + m_name + "?");

    QVBoxLayout* vertical_layout = new QVBoxLayout;
    QHBoxLayout* browse_layout = new QHBoxLayout;

    path_text = new QLineEdit(m_q_path, this);
    QPushButton* browse_btn = new QPushButton(tr("Browse"), this);

    browse_layout->addWidget(path_text);
    browse_layout->addWidget(browse_btn);

    QFormLayout* layout = new QFormLayout;
    QLabel* text = new QLabel("File was not found in the saved path or could not be read.\nIt might have been moved.\nEnter the new path to the file.");
    layout->addWidget(text);
    layout->addRow("Path:", browse_layout);
    QPushButton* path_btn = new QPushButton(tr("Mange paths..."), this);
    layout->addWidget(path_btn);

    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    vertical_layout->addLayout(layout);
    vertical_layout->addWidget(btn_box);
    setLayout(vertical_layout);

    connect(browse_btn, &QPushButton::clicked, this, &PathDialog::browse_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &PathDialog::ok_btn_clicked);
    connect(path_btn, &QPushButton::clicked, this, &PathDialog::path_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &PathDialog::cancel_btn_clicked);
}


void PathDialog::enable_ok_btn(const bool& enable) {
    btn_box->button(QDialogButtonBox::Ok)->setEnabled(enable);
}

void PathDialog::browse_btn_clicked() {
    QDir standard;
    standard.mkpath(m_default_path);
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Choose project path"),
                                                m_q_path);
    if(!file.isEmpty()) {
        path_text->setText(file);
    }
}

void PathDialog::path_btn_clicked() {
    close();
    emit open_view_path_dialog();
}

void PathDialog::ok_btn_clicked() {
    *m_path = path_text->text().toStdString();
    accept();
}

void PathDialog::cancel_btn_clicked() {
    reject();
}

