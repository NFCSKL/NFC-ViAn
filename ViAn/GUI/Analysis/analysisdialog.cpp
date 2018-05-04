#include "analysisdialog.h"

AnalysisDialog::AnalysisDialog(std::vector<VideoItem *> vid_projs, AnalysisSettings *analysis_settings)
    : m_analysis_settings(analysis_settings) {
    // Set title
    setWindowTitle("Vian - Advanced analysis");
    // Remove "?" button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    // Box layout to store video projects
    QVBoxLayout* v_lay = new QVBoxLayout();
    if (vid_projs.empty()) do_analysis = false;

    if (do_analysis) {
        m_v_proj_list = new QListWidget(this);

        // Windows-like file selection
        m_v_proj_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
        for(VideoItem* v : vid_projs) {
            VideoListItem* item = new VideoListItem(v->get_video_project());
            m_v_proj_list->addItem(item);
        }
        v_lay->addWidget(m_v_proj_list);

        connect(m_v_proj_list, &QListWidget::itemSelectionChanged, this, &AnalysisDialog::item_changed);
    }

    // Add analysis form
    QFormLayout* form_lay = new QFormLayout();
    v_lay->addLayout(form_lay);
    // Add analysis specific options
    //VideoListItem* item  = dynamic_cast<VideoListItem*>(m_v_proj_list->itemAt(0,0));
    //method = new MotionDetection(item->m_vid_proj->get_video()->file_path, m_save_dir);
    add_settings(form_lay);
    // Add Buttons
    btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    btn_box->addButton(QDialogButtonBox::Reset);
    if (do_analysis) btn_box->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AnalysisDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &AnalysisDialog::cancel_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &AnalysisDialog::reset_settings);

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
    if (do_analysis) {
        for(auto item : m_v_proj_list->selectedItems()){
            VideoProject* vid_proj = dynamic_cast<VideoListItem*>(item)->m_vid_proj;
            //method = new MotionDetection(vid_proj->get_video()->file_path, m_save_dir);
            //set_settings(method);
            set_settings();
            //emit start_analysis(method, vid_proj);
            emit start_analysis(vid_proj, m_analysis_settings);
            close();
            return;
        }
    }
    set_settings();
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
    //std::vector<std::string> vars = method->get_var_names();
    std::vector<std::string> vars = m_analysis_settings->get_var_names();
    for(std::string name : vars) {
        QWidget* var_line;
        if (name == "DETECT_SHADOWS") {
            var_line = new QCheckBox();
        } else {
            var_line = new QLineEdit(QString::number(m_analysis_settings->get_setting(name)));
        }
        var_line->setToolTip(QString::fromStdString(m_analysis_settings->get_descr(name)));
        form->addRow(QString::fromStdString(name), var_line);
        m_settings.insert(std::make_pair(name, var_line));
    }
}

/**
 * @brief AnalysisDialog::set_settings
 * @param method
 * Set form layout settings to method
 */
void AnalysisDialog::set_settings() {
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
        //method->set_setting(line.first, val);
        m_analysis_settings->set_setting(line.first, val);
    }
}

void AnalysisDialog::reset_settings() {
    m_analysis_settings->reset_settings();
    for (auto line : m_settings) {
        if (line.first == "DETECT_SHADOWS") {
            QCheckBox* settings = dynamic_cast<QCheckBox*>(line.second);
            settings->setChecked(m_analysis_settings->get_setting(line.first));
        } else {
            QLineEdit* settings = dynamic_cast<QLineEdit*>(line.second);
            settings->setText(QString::number(m_analysis_settings->get_setting(line.first)));
        }
    }
}
