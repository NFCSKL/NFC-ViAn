#include "generatevideodialog.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>

GenerateVideoDialog::GenerateVideoDialog(std::vector<QSize> sizes, std::vector<int> fpses, QWidget* parent) : QDialog(parent) {
    // Set title
    setWindowTitle("Vian - New video");
    // Remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(200, 100);

    // Setup widget layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    name_edit = new QLineEdit(this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    // Name line
    QFormLayout* video_name_layout = new QFormLayout;
    video_name_layout->addRow("Video name: ", name_edit);

    // Resolution line
    resolution = new QComboBox;
    for (QSize size : sizes) {
        QString text = QString::number(size.width()) + "x" + QString::number(size.height());
        resolution->addItem(text, size);
    }
    QFormLayout* resolution_layout = new QFormLayout;
    resolution_layout->addRow("Resolution: ", resolution);

    // Frame rate line
    frame_rate = new QComboBox;
    for (int fps : fpses) {
        QString text = QString::number(fps);
        frame_rate->addItem(text, fps);
    }
    QFormLayout* frame_rate_layout = new QFormLayout;
    frame_rate_layout->addRow("Frame rate: ", frame_rate);

    // Add all to the layout
    vertical_layout->addLayout(video_name_layout);
    vertical_layout->addLayout(resolution_layout);
    vertical_layout->addLayout(frame_rate_layout);
    vertical_layout->addWidget(btn_box);

    // Connect buttons
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &GenerateVideoDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &GenerateVideoDialog::cancel_btn_clicked);
    setLayout(vertical_layout);
}

GenerateVideoDialog::~GenerateVideoDialog() {
    delete name_edit;
    delete btn_box;
}

void GenerateVideoDialog::get_values(QString* name, QSize* size, int* fps) {
    *name = name_edit->text();
    *size = resolution->currentData().toSize();
    *fps = frame_rate->currentData().toInt();
}

/**
 * @brief GenerateVideoDialog::ok_btn_clicked
 * Accept widget and return variables
 */
void GenerateVideoDialog::ok_btn_clicked() {
    QSize size = resolution->currentData().toSize();
    qDebug() << size.width() << size.height();
    int fps = frame_rate->currentData().toInt();
    qDebug() << fps;
    accept();
}

/**
 * @brief GenerateVideoDialog::cancel_btn_clicked
 * Close widget
 */
void GenerateVideoDialog::cancel_btn_clicked() {
    reject();
}
