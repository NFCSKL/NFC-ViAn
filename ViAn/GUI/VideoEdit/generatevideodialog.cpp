#include "generatevideodialog.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include <QDebug>

GenerateVideoDialog::GenerateVideoDialog(std::vector<QSize> sizes, std::vector<int> fpses, QWidget* parent) : QDialog(parent) {
    // Set title
    setWindowTitle("Vian - New video");
    // Remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(300, 100);

    // Setup widget layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    name_edit = new QLineEdit(this);
    name_edit->setText("video");
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

    custom_size_box = new QCheckBox;
    connect(custom_size_box, &QCheckBox::stateChanged, this, &GenerateVideoDialog::custom_size_toggled);
    custom_width = new QLineEdit(this);
    custom_height = new QLineEdit(this);
    QValidator* size_validator = new QIntValidator(100, 4000, this);
    custom_width->setValidator(size_validator);
    custom_height->setValidator(size_validator);

    QHBoxLayout* custom_size_layout = new QHBoxLayout();
    custom_size_layout->addWidget(custom_width);
    custom_size_layout->addWidget(custom_height);

    resolution_layout->addRow("Custom resolution: ", custom_size_box);
    resolution_layout->addItem(custom_size_layout);

    // Keep size option
    keep_size_box = new QCheckBox;
    connect(keep_size_box, &QCheckBox::stateChanged, this, &GenerateVideoDialog::keep_size_toggled);
    keep_size_box->setChecked(true);
    QFormLayout* keep_size_layout = new QFormLayout;
    keep_size_layout->addRow("Keep default resolutions ", keep_size_box);

    // Frame rate line
    frame_rate = new QComboBox;
    for (int fps : fpses) {
        QString text = QString::number(fps);
        frame_rate->addItem(text, fps);
    }
    QFormLayout* frame_rate_layout = new QFormLayout;
    frame_rate_layout->addRow("Frame rate: ", frame_rate);

    custom_fps_box = new QCheckBox;
    connect(custom_fps_box, &QCheckBox::stateChanged, this, &GenerateVideoDialog::custom_fps_toggled);
    custom_fps = new QLineEdit(this);
    QValidator* fps_validator = new QIntValidator(5, 60, this);
    custom_fps->setValidator(fps_validator);
    custom_fps->setDisabled(true);

    frame_rate_layout->addRow("Custom frame rate: ", custom_fps_box);
    frame_rate_layout->addWidget(custom_fps);

    // Title screen line
    title_box = new QCheckBox;
    connect(title_box, &QCheckBox::stateChanged, this, &GenerateVideoDialog::title_screen_toggled);
    title_text = new QLineEdit(this);
    title_text->setMaxLength(20);
    title_text->setDisabled(true);

    QFormLayout* title_screen_layout = new QFormLayout;
    title_screen_layout->addRow("Add title screen: ", title_box);
    title_screen_layout->addWidget(title_text);

    // Add all to the layout
    vertical_layout->addLayout(video_name_layout);
    vertical_layout->addLayout(resolution_layout);
    vertical_layout->addLayout(keep_size_layout);
    vertical_layout->addLayout(frame_rate_layout);
    vertical_layout->addLayout(title_screen_layout);
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

void GenerateVideoDialog::get_values(QString* name, QSize* size, int* fps,
                                     bool* keep_size, bool* title_screen, QString* text) {
    if (custom_size_box->isChecked()) {
        int width = custom_width->text().toInt();
        int height = custom_height->text().toInt();
        *size = QSize(width, height);
    } else {
        *size = resolution->currentData().toSize();
    }

    if (custom_fps_box->isChecked()) {
        *fps = custom_fps->text().toInt();
    } else {
        *fps = frame_rate->currentData().toInt();
    }

    *name = name_edit->text();
    *keep_size = keep_size_box->checkState();

    *title_screen = title_box->checkState();
    *text = title_text->text();
}

void GenerateVideoDialog::keep_size_toggled(int state) {
    resolution->setDisabled(state);
    custom_size_box->setDisabled(state);
    custom_width->setDisabled(state);
    custom_height->setDisabled(state);
    if (!state) {
        custom_size_toggled(custom_size_box->checkState());
    }
}

void GenerateVideoDialog::custom_size_toggled(int state) {
    resolution->setDisabled(state);
    custom_width->setEnabled(state);
    custom_height->setEnabled(state);
}

void GenerateVideoDialog::custom_fps_toggled(int state) {
    frame_rate->setDisabled(state);
    custom_fps->setEnabled(state);
}

void GenerateVideoDialog::title_screen_toggled(int state) {
    title_text->setEnabled(state);
}

/**
 * @brief GenerateVideoDialog::ok_btn_clicked
 * Accept widget and return variables
 */
void GenerateVideoDialog::ok_btn_clicked() {
    if (custom_size_box->isChecked()) {
        if (!custom_width->hasAcceptableInput() || !custom_height->hasAcceptableInput()) {
            QMessageBox msg_box;
            msg_box.setText("Invalid resolution");
            msg_box.setDefaultButton(QMessageBox::Ok);
            msg_box.exec();
            return;
        }
    }
    if (custom_fps_box->isChecked()) {
        if (!custom_fps->hasAcceptableInput()) {
            QMessageBox msg_box;
            msg_box.setText("Invalid frame rate");
            msg_box.setDefaultButton(QMessageBox::Ok);
            msg_box.exec();
            return;
        }
    }
    accept();
}

/**
 * @brief GenerateVideoDialog::cancel_btn_clicked
 * Close widget
 */
void GenerateVideoDialog::cancel_btn_clicked() {
    reject();
}
