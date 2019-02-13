#include "frameexporterdialog.h"

#include <QBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>

#include "imageexporter.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/video.h"


FrameExporterDialog::FrameExporterDialog(ImageExporter* im_exp, Video* video, QString proj_path,
                                         const int& max_frame, const std::pair<int, int>& interval,
                                         QWidget* parent) : QDialog(parent){
    m_exporter = im_exp;
    m_video_name = video->get_name();
    m_old_path_text = proj_path + Constants::STILLS_FOLDER;
    setWindowTitle("ViAn - Image exporter");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/Icons/folder_interval.png"));

    m_from_box = new QSpinBox(this);
    m_from_box->setRange(0, max_frame);
    m_to_box = new QSpinBox(this);
    m_to_box->setRange(0, max_frame);
    m_total = new QLabel(this);

    m_from_box->setValue(interval.first);
    m_to_box->setValue(interval.second);
    update_total();

    connect(m_from_box, &QSpinBox::editingFinished, this, &FrameExporterDialog::from_value_changed);
    connect(m_to_box, &QSpinBox::editingFinished, this, &FrameExporterDialog::to_value_changed);
    // Frame layout
    m_frame_input_layout = new QHBoxLayout();
    m_frame_input_layout->addWidget(new QLabel(tr("from:")));
    m_frame_input_layout->addWidget(m_from_box);
    m_frame_input_layout->addWidget(new QLabel(tr("to:")));
    m_frame_input_layout->addWidget(m_to_box);
    m_frame_input_layout->addWidget(new QLabel(tr("total:")));
    m_frame_input_layout->addWidget(m_total);
    // End frame layout

    m_path_label = new QLabel(m_old_path_text, this);
    m_browse_btn = new QPushButton("Browse", this);
    connect(m_browse_btn, &QPushButton::clicked, this, &FrameExporterDialog::open_path_dialog);
    // Path layout
    m_path_input_layout = new QHBoxLayout();
    m_path_input_layout->addWidget(m_path_label);
    m_path_input_layout->addWidget(m_browse_btn);
    // End path layour

    m_export_btn = new QPushButton("Export", this);
    m_btn_box = new QDialogButtonBox(QDialogButtonBox::Cancel);
    m_btn_box->addButton(m_export_btn, QDialogButtonBox::AcceptRole);
    connect(m_btn_box, &QDialogButtonBox::accepted, this, &FrameExporterDialog::save_values);
    connect(m_btn_box, &QDialogButtonBox::rejected, this, &FrameExporterDialog::reject);
    if (video->is_sequence()) {
        ImageSequence* sequence = dynamic_cast<ImageSequence*>(video);
        m_video_path = new QLabel(sequence->get_search_path());
    } else {
        m_video_path = new QLabel(video->file_path);
    }
    m_path = video->file_path;
    // Input layout
    m_input_layout = new QFormLayout(this);
    m_input_layout->addRow(new QLabel(tr("Video"), this), m_video_path);
    m_input_layout->addRow(new QLabel(tr("Frames")), m_frame_input_layout);
    m_input_layout->addRow(new QLabel(tr("Destination")), m_path_input_layout);
    m_input_layout->addWidget(m_btn_box);
    // End input layout
}

/**
 * @brief FrameExporterDialog::update_total
 * Updates the label showing the total amount of frames to be exported
 */
void FrameExporterDialog::update_total(){
    m_total->setText(QString::number(m_to_box->value() - m_from_box->value() + 1));
}

/**
 * @brief FrameExporterDialog::from_value_changed
 * Forces the value of m_to_box to increase if the new value is bigger
 * @param value :   new value of m_from_box
 */
void FrameExporterDialog::from_value_changed() {
    if (m_from_box->value() > m_from_box->maximum())
        m_from_box->setValue(m_from_box->maximum());
    else if (m_from_box->value() < m_from_box->minimum())
        m_from_box->setValue((m_from_box->minimum()));

    if (m_from_box->value() >= m_to_box->value()) m_to_box->setValue(m_from_box->value());
    update_total();
}

/**
 * @brief FrameExporterDialog::to_value_changed
 * Forces the value of m_from_box to decrease if the new value is smaller
 * @param value :   new value of m_to_box
 */
void FrameExporterDialog::to_value_changed() {
    if (m_to_box->value() > m_to_box->maximum())
        m_to_box->setValue(m_to_box->maximum());
    else if (m_from_box->value() < m_from_box->minimum())
        m_to_box->setValue(m_to_box->minimum());

    if (m_to_box->value() <= m_from_box->value()) m_from_box->setValue(m_to_box->value());
    update_total();
}

/**
 * @brief FrameExporterDialog::open_path_dialog
 * Sets the export path from the file dialog
 */
void FrameExporterDialog::open_path_dialog() {
    QString export_path = QFileDialog::getExistingDirectory(
                this, "Select export path", m_path_label->text(), QFileDialog::ShowDirsOnly);
    if (!export_path.isEmpty()) {
        m_path_label->setText(export_path);
    }
}

/**
 * @brief FrameExporterDialog::save_values
 * Called when acceptrole button is clicked.
 * Does a path check and saves the entered values in
 * the ImageExporter pointer
 */
void FrameExporterDialog::save_values() {
    if (!check_path()) {
        return;
    }
    QString e_path = m_path_label->text() + "/" + m_video_name + "_";
    m_exporter->set_interval(std::make_pair(m_from_box->value(), m_to_box->value()));
    m_exporter->set_export_path(e_path);
    m_exporter->set_file_path(m_path);
    accept();
}

/**
 * @brief FrameExporterDialog::check_path
 * Asks the user if the export path should be created
 * given that it does not already exist
 * @return true if the path exists or has been created
 */
bool FrameExporterDialog::check_path() {
    if (QDir(m_path_label->text()).exists()) return true;
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Warning);
    msg_box.setMinimumSize(500,120);
    msg_box.setText("Path '" + m_path_label->text() + "' doesn't exist");
    msg_box.setInformativeText("Do you wish to create it?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (msg_box.exec() == QMessageBox::No) return false;
    return QDir().mkpath(m_path_label->text());
}
