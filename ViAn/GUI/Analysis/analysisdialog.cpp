#include "analysisdialog.h"

AnalysisDialog::AnalysisDialog(std::vector<VideoItem *> vid_projs, std::string save_dir) : m_save_dir(save_dir)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout* v_lay = new QVBoxLayout();
    m_v_proj_list = new QListWidget(this);
    m_v_proj_list->setSelectionMode(QAbstractItemView::ExtendedSelection );
    for(VideoItem* v : vid_projs){
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
    QDialogButtonBox* btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AnalysisDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &AnalysisDialog::cancel_btn_clicked);

    v_lay->addWidget(btn_box);
    setLayout(v_lay);
}

void AnalysisDialog::ok_btn_clicked()
{
    for(auto item : m_v_proj_list->selectedItems()){
        VideoProject* vid_proj = dynamic_cast<VideoListItem*>(item)->m_vid_proj;
        method = new MotionDetection(vid_proj->get_video()->file_path, m_save_dir);
        set_settings(method);
        emit start_analysis(method, vid_proj);
    }
    close();
}

void AnalysisDialog::cancel_btn_clicked()
{
    close();
}

void AnalysisDialog::add_settings(QFormLayout *form)
{
    std::vector<std::string> vars = method->get_var_names();
    for(std::string name : vars){
        QLineEdit* var_line = new QLineEdit(QString::fromStdString(std::to_string(method->get_setting(name))));
        var_line->setToolTip(QString::fromStdString(method->get_descr(name)));
        form->addRow(QString::fromStdString(name), var_line);
        m_settings.insert(std::make_pair(name,var_line));
    }
}

void AnalysisDialog::set_settings(AnalysisMethod *method)
{
    for(auto line: m_settings){
        QLineEdit* setting = line.second;
        int val = std::stoi(setting->text().toStdString());
        method->set_setting(line.first,val);
    }
}
