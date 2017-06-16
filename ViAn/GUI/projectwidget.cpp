#include "projectwidget.h"
#include "makeproject.h"
#include "createprojectdialog.h"
#include "TreeItems/videoitem.h"

#include <QDebug>

ProjectWidget::ProjectWidget(ProjectManager* project_manager, QWidget *parent) : QTreeWidget(parent) {
    m_project_manager = project_manager;
}

void ProjectWidget::new_project() {
    if (proj == nullptr) {
        CreateProjectDialog* proj_dialog = new CreateProjectDialog();
        QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
    } else {
        // TODO project already loaded
    }
}

void ProjectWidget::add_project(QString project_name, QString project_path) {
    std::string _tmp_name = project_name.toStdString();
    std::string _tmp_path = project_path.toStdString();
    proj = m_project_manager->create_project(_tmp_name, _tmp_path, _tmp_path);
    create_default_tree();
}

void ProjectWidget::create_default_tree() {
    videos = new QTreeWidgetItem();
    videos->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    videos->setText(0, tr("Videos"));
    addTopLevelItem(videos);
}

void ProjectWidget::add_video() {
    if (proj != nullptr) {
        QString video_path = QFileDialog().getOpenFileName(this, tr("Add video"), "C:"); //TODO fix initial path

        int index = video_path.lastIndexOf('/') + 1;
        tree_add_video(video_path.right(video_path.length()-index));
    }
}

void ProjectWidget::tree_add_video(QString vid_name) {
    VideoProject* vid_proj = new VideoProject(new Video(vid_name.toStdString()));
    VideoItem* vid = new VideoItem(vid_proj);
    vid->setText(0, vid_name);
    videos->addChild(vid);
    //TODO emit selected media on item clicked

}
