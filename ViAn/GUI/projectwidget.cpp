#include "projectwidget.h"
#include "projectdialog.h"
#include "TreeItems/itemtypes.h"
#include <QFileDialog>
#include <QDebug>

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this , SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
}

/**
 * @brief ProjectWidget::new_project
 * Creates a create project dialog
 */
void ProjectWidget::new_project() const {
    if (m_proj == nullptr) {
        ProjectDialog* proj_dialog = new ProjectDialog();
        QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
    } else {
        // TODO project already loadedq
    }
}

/**
 * @brief ProjectWidget::add_project
 * Slot function called by the create project dialog.
 * Creates a new project and generates the default tree structure
 * @param project_name
 * @param project_path
 */
void ProjectWidget::add_project(QString project_name, QString project_path) {
    std::string _tmp_name = project_name.toStdString();
    std::string _tmp_path = project_path.toStdString();    
    m_proj = new Project(_tmp_name, _tmp_path);
    create_default_tree();
}

/**
 * @brief ProjectWidget::create_default_tree
 * Creates and adds the default tree structure
 */
void ProjectWidget::create_default_tree() {
    m_videos = new FolderItem(FOLDER_ITEM);
    m_videos->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    m_videos->setText(0, tr("Videos"));
    addTopLevelItem(m_videos);
}

/**
 * @brief ProjectWidget::add_video
 * Slot function to add a video.
 * Creates a file dialog and creates a video project based on file path
 */
void ProjectWidget::add_video() {
    if (m_proj == nullptr)  return; // TODO: HANDLE CASE

    QString video_path = QFileDialog().getOpenFileName(this, tr("Add video"), m_proj->getDir_videos().c_str());
    int index = video_path.lastIndexOf('/') + 1;
    QString vid_name = video_path.right(video_path.length() - index);

    VideoProject* vid_proj = new VideoProject(new Video(video_path.toStdString()));
    m_proj->add_video_project(vid_proj);

    tree_add_video(vid_proj, vid_name);
}

/**
 * @brief ProjectWidget::start_analysis
 * @param vid_proj
 * Start analysis on the selected video
 */
void ProjectWidget::start_analysis(VideoProject* vid_proj) {
    AnalysisItem* ana = new AnalysisItem(ANALYSIS_ITEM);
    for (int i = 0; i < m_videos->childCount(); i++) {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(m_videos->child(i));
        if (vid_item->get_video_project() == vid_proj) {
            m_videos->child(i)->addChild(ana);
            ana->setText(0, "Loading");
            m_videos->child(i)->setExpanded(true);
            QTreeWidgetItem* item = dynamic_cast<QTreeWidgetItem*>(ana);
            emit begin_analysis(m_proj->getDir(), vid_proj->get_video()->file_path, item);
        }
    }
}

/**
 * @brief ProjectWidget::set_tree_item_name
 * @param item
 * @param name
 * Slot to set the name if an item in the project tree
 */
void ProjectWidget::set_tree_item_name(QTreeWidgetItem* item, QString name) {
    item->setText(0, name);
}

/**
 * @brief ProjectWidget::tree_add_video
 * Adds the video to the project tree
 * @param vid_proj
 * @param vid_name
 */
void ProjectWidget::tree_add_video(VideoProject* vid_proj, const QString& vid_name) {
    VideoItem* vid = new VideoItem(vid_proj, VIDEO_ITEM);
    vid->setText(0, vid_name);
    m_videos->addChild(vid);
    emit set_status_bar("Video added");
    m_videos->setExpanded(true);
}

/**
 * @brief ProjectWidget::tree_item_clicked
 * Slot function for when a tree item is clicked.
 * Preforms different operations based on tree item type.
 * @param item
 * @param col
 */
void ProjectWidget::tree_item_clicked(QTreeWidgetItem* item, const int& col) {
    switch(item->type()){
    case VIDEO_ITEM: {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        emit marked_video(vid_item->get_video_project());
        emit set_detections(false);
        emit enable_poi_btns(false);
        break;
    } case ANALYSIS_ITEM: {
        tree_item_clicked(item->parent(), 0);
        AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(item);
        emit marked_analysis(ana_item->get_analysis());
        emit set_detections(true);
        if (!ana_item->get_analysis()->POIs.empty()) {
            emit enable_poi_btns(true);
        }
        break;
    } case FOLDER_ITEM: {
        break;
    } default:
        break;
    }

}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
void ProjectWidget::save_project() {
    m_proj->save_project();
    emit set_status_bar("Project saved");
}

/**
 * @brief ProjectWidget::open_project
 * Slot function to open a previously created project
 */
void ProjectWidget::open_project() {
    QString project_path = QFileDialog().getOpenFileName(this, tr("Open project"), QDir::homePath());
    if (!project_path.isEmpty()) {
        emit set_status_bar("Opening project");
        clear();
        create_default_tree();
        m_proj = Project::fromFile(project_path.toStdString());
        for (auto vid_pair : m_proj->get_videos()) {
            VideoProject* vid_proj = vid_pair.second;
            QString video_path = QString::fromStdString(vid_proj->get_video()->file_path);
            int index = video_path.lastIndexOf('/') + 1;
            QString vid_name = video_path.right(video_path.length() - index);
            tree_add_video(vid_proj, vid_name);
        }
    }
}

/**
 * @brief ProjectWidget::close_project
 * TODO Fix
 */
void ProjectWidget::close_project() {
    emit set_status_bar("TODO - Closed the project");
}

/**
 * @brief ProjectWidget::remove_project
 * TODO FIX
 */
void ProjectWidget::remove_project() {
    emit set_status_bar("TODO - Removed the project");
}
