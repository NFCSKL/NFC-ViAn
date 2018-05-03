#include "analysisdialog.h"

AnalysisDialog::AnalysisDialog(std::vector<VideoItem *> vid_projs, std::string save_dir)
    : m_save_dir(save_dir) {
    // Set title
    setWindowTitle("Vian - Advanced analysis");
    // Remove "?" button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    // Box layout to store video projects
    QVBoxLayout* v_lay = new QVBoxLayout();
    m_v_proj_list = new QListWidget(this);

    // Windows-like file selection
    m_v_proj_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for(VideoItem* v : vid_projs) {
        VideoListItem* item = new VideoListItem(v->get_video_project());
        m_v_proj_list->addItem(item);
    }
    v_lay->addWidget(m_v_proj_list);

    // Add analysis form
    QFormLayout* form_lay = new QFormLayout();
    v_lay->addLayout(form_lay);
    // Add analysis specific options
    VideoListItem* item  = dynamic_cast<VideoListItem*>(m_v_proj_list->itemAt(0,0));
    method = new MotionDetection(item->m_vid_proj->get_video()->file_path, m_save_dir);
    add_settings(form_lay);
    // Add Buttons
    btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AnalysisDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &AnalysisDialog::cancel_btn_clicked);
    connect(m_v_proj_list, &QListWidget::itemSelectionChanged, this, &AnalysisDialog::item_changed);

    v_lay->addWidget(btn_box);
    setLayout(v_lay);
}

AnalysisDialog::~AnalysisDialog() {

}

/**
 * @brief AnalysisDialog::ok_btn_clicked
 * Okay clicked, if any selected videos, execute analyses for them
 */
void AnalysisDialog::ok_btn_clicked() {
    for(auto item : m_v_proj_list->selectedItems()){
        VideoProject* vid_proj = dynamic_cast<VideoListItem*>(item)->m_vid_proj;
        method = new MotionDetection(vid_proj->get_video()->file_path, m_save_dir);
        set_settings(method);
        emit start_analysis(method, vid_proj);
    }
    close();
}

/**
 * @brief AnalysisDialog::cancel_btn_clicked
 * Cancel clicked, close window
 */
void AnalysisDialog::cancel_btn_clicked() {
    close();
}

void AnalysisDialog::item_changed() {
    if (m_v_proj_list->selectedItems().length() > 0) {
        btn_box->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        btn_box->button(QDialogButtonBox::Ok)->setDisabled(true);
    }
}

/**
 * @brief AnalysisDialog::add_settings
 * @param form
 * Add Settings to form layout
 */
void AnalysisDialog::add_settings(QFormLayout *form) {
    std::vector<std::string> vars = method->get_var_names();
    for(std::string name : vars) {
        QWidget* var_line;
        if (name == "DETECT_SHADOWS") {
            var_line = new QCheckBox();
        } else {
            var_line = new QLineEdit(QString::number(method->get_setting(name)));
        }
        var_line->setToolTip(QString::fromStdString(method->get_descr(name)));
        form->addRow(QString::fromStdString(name), var_line);
        m_settings.insert(std::make_pair(name, var_line));
    }
}

/**
 * @brief AnalysisDialog::set_settings
 * @param method
 * Set form layout settings to method
 */
void AnalysisDialog::set_settings(AnalysisMethod *method) {
    for(auto line : m_settings) {
        int val;
        if (line.first == "DETECT_SHADOWS") {
            QCheckBox* settings = dynamic_cast<QCheckBox*>(line.second);
            val = settings->isChecked();
        } else {
            QLineEdit* settings = dynamic_cast<QLineEdit*>(line.second);
            val = settings->text().toInt();
        }
        qDebug() << QString::fromStdString(line.first) << val;
        method->set_setting(line.first, val);
    }
}
