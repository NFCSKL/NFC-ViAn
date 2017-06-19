#include "projectwidget.h"
#include "createprojectdialog.h"
#include "TreeItems/itemtypes.h"
#include <QFileDialog>
#include <QDebug>

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    add_analysis();
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this , SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
}

void ProjectWidget::new_project() {
    if (m_proj == nullptr) {
        CreateProjectDialog* proj_dialog = new CreateProjectDialog();
        QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
    } else {
        // TODO project already loadedq
    }
}

void ProjectWidget::add_project(QString project_name, QString project_path) {
    std::string _tmp_name = project_name.toStdString();
    std::string _tmp_path = project_path.toStdString();    
    m_proj = new Project(_tmp_name, _tmp_path);
    m_proj->save_project();
    create_default_tree();
}

void ProjectWidget::create_default_tree() {
    m_videos = new QTreeWidgetItem();
    m_videos->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    m_videos->setText(0, tr("Videos"));
    addTopLevelItem(m_videos);
}

void ProjectWidget::add_video() {
    if (m_proj == nullptr)  return; // TODO: HANDLE CASE

    QString video_path = QFileDialog().getOpenFileName(this, tr("Add video"), m_proj->getDir_videos().c_str());

    int index = video_path.lastIndexOf('/') + 1;
    QString vid_name = video_path.right(video_path.length() - index);

    VideoProject* vid_proj = new VideoProject(new Video(video_path.toStdString()));
    m_proj->add_video_project(vid_proj);

    tree_add_video(vid_proj, vid_name);
}

void ProjectWidget::add_analysis(){

    tree_add_analysis();
}

void ProjectWidget::tree_add_analysis(){
    AnalysisItem* ana = new AnalysisItem(ANALYSIS_ITEM);
    addTopLevelItem(ana);
    ana->setText(0,"aina");
}

void ProjectWidget::tree_add_video(VideoProject* vid_proj, QString vid_name) {
    VideoItem* vid = new VideoItem(vid_proj, VIDEO_ITEM);
    vid->setText(0, vid_name);
    m_videos->addChild(vid);
}


void ProjectWidget::tree_item_clicked(QTreeWidgetItem* item, int col) {
    switch(item->type()){
    case VIDEO_ITEM:
        qDebug() << "Video Selected";
        break;
    case ANALYSIS_ITEM:
        qDebug() << "Analysis Selected";
        break;
    default:
        qDebug() << "Nothing clicked";
    }

}
