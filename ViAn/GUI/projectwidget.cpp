#include "projectwidget.h"
#include "projectdialog.h"
#include "GUI/TreeItems/analysisitem.h"
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QFileInfo>
#include <QDirIterator>
#include <QShortcut>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "Project/projecttreestate.h"
#include "Project/recentproject.h"

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

    connect(this, &ProjectWidget::customContextMenuRequested, this, &ProjectWidget::context_menu);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this , SLOT(tree_item_clicked(QTreeWidgetItem*,int)));

    // Widget only shortcut for creating a new folder
    QShortcut* new_folder_sc = new QShortcut(this);
    new_folder_sc->setContext(Qt::WidgetWithChildrenShortcut);
    new_folder_sc->setKey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    connect(new_folder_sc, &QShortcut::activated, this, &ProjectWidget::create_folder_item);

    connect(this, &ProjectWidget::itemSelectionChanged, this , &ProjectWidget::check_selection);
    connect(this, &ProjectWidget::currentItemChanged, this, &ProjectWidget::check_selection_level);
}


/**
 * @brief ProjectWidget::new_project
 * Creates a create project dialog
 */
void ProjectWidget::new_project() {
    ProjectDialog* proj_dialog = new ProjectDialog();
    QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
}

/**
 * @brief ProjectWidget::add_project
 * Slot function called by the create project dialog.
 * Creates a new project and generates the default tree structure
 * @param project_name
 * @param project_path
 */
void ProjectWidget::add_project(QString project_name, QString project_path) {
    close_project();
    std::string _tmp_name = project_name.toStdString();
    std::string _tmp_path = project_path.toStdString();
    parentWidget()->parentWidget()->setWindowTitle(project_name);
    m_proj = new Project(_tmp_name, _tmp_path); 
    m_proj->save_project();
    _tmp_path.append(_tmp_name);
    emit proj_path(m_proj->getDir());
}

/**
 * @brief ProjectWidget::add_video
 * Slot function to add a video.
 * Creates a file dialog and creates a video project based on file path
 */
void ProjectWidget::add_video() {
    if (m_proj == nullptr)  return;
    // TODO: HANDLE CASE. Only open video files
    QStringList video_paths = QFileDialog().getOpenFileNames(this, tr("Add video"), m_proj->getDir().c_str());
    for (auto video_path : video_paths){
        int index = video_path.lastIndexOf('/') + 1;
        QString vid_name = video_path.right(video_path.length() - index);
        // TODO Check if file is already added
        VideoProject* vid_proj = new VideoProject(new Video(video_path.toStdString()));
        m_proj->add_video_project(vid_proj);
        tree_add_video(vid_proj, vid_name);
    }
}

/**
 * @brief ProjectWidget::start_analysis
 * @param vid_proj
 * Start analysis on the selected video
 */
void ProjectWidget::start_analysis(VideoProject* vid_proj, AnalysisSettings* settings) {
    AnalysisMethod* method = new MotionDetection(vid_proj->get_video()->file_path, m_proj->getDir());
    if(settings->use_bounding_box) method->setBounding_box(settings->bounding_box);
    if(settings->use_interval) method->setInterval(settings->interval);

    if (vid_proj == nullptr) return;
    VideoItem* v_item = get_video_item(vid_proj);
    AnalysisItem* ana = new AnalysisItem();
    v_item->addChild(ana);
    ana->setText(0, "Loading");
    v_item->setExpanded(true);
    QTreeWidgetItem* item = dynamic_cast<QTreeWidgetItem*>(ana);    
    emit begin_analysis(item, method);
}

/**
 * @brief ProjectWidget::add_tag
 * @param vid_proj
 * @param tag
 * Adds a tag 'tag' under vid_proj
 */
void ProjectWidget::add_basic_analysis(VideoProject* vid_proj, BasicAnalysis* tag) {
    TagItem* tag_item = new TagItem(dynamic_cast<Tag*>(tag));
    vid_proj->add_analysis(tag);

    VideoItem* vid_item = get_video_item(vid_proj);
    if (vid_item == nullptr) {
        set_status_bar("Something went wrong when adding tag: " + QString::fromStdString(tag->get_name()));
        return;
    }

    vid_item->addChild(tag_item);
    vid_item->setExpanded(true);
}

/**
 * @brief ProjectWidget::set_tree_item_namebei
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
    VideoItem* vid_item = new VideoItem(vid_proj);
    insertTopLevelItem(topLevelItemCount(), vid_item);
    vid_proj->set_tree_index(get_index_path(dynamic_cast<QTreeWidgetItem*>(vid_item)));
    emit set_status_bar("Video added: " + vid_name);
    // Add analysis and tag
    add_analyses_to_item(vid_item);
}

/**
 * @brief ProjectWidget::mimeTypes
 * @return Accepted mime types
 */
QStringList ProjectWidget::mimeTypes() const {
    // Appended with 'text/uri-list' to allow file drops
    return QStringList() << "application/x-qabstractitemmodeldatalist" << "text/uri-list";
}

/**
 * @brief ProjectWidget::file_dropped
 * Loads the video into the program if it is of accepted type
 * @param path  : path to the video file
 */
void ProjectWidget::file_dropped(QString path) {
    std::set<std::string> exts {"mkv", "flv", "vob", "ogv", "ogg",
                                "264", "263", "mjpeg", "avc", "m2ts",
                                "mts", "avi", "mov", "qt", "wmv", "mp4",
                                "m4p", "m4v", "mpg", "mp2", "mpeg",
                                "mpe", "mpv", "m2v", "m4v", "3gp", "3g2",
                                "flv", "f4v", "f4p", "f4a", "f4b"};
    QFileInfo tmp(path);
    std::string ext = tmp.suffix().toStdString();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (exts.find(ext) != exts.end()) {
        // Add file
        int index = path.lastIndexOf('/') + 1;
        QString vid_name = path.right(path.length() - index);

        VideoProject* vid_proj = new VideoProject(new Video(path.toStdString()));
        m_proj->add_video_project(vid_proj);
        VideoItem* v_item = new VideoItem(vid_proj);
        insertTopLevelItem(topLevelItemCount(), v_item);
        vid_proj->set_tree_index(get_index_path(v_item));
    }

}

/**
 * @brief ProjectWidget::folder_dropped
 * Searches the path for folders and files
 * @param path  : path to the folder
 */
void ProjectWidget::folder_dropped(QString path) {
    QDirIterator d_iter(path);
    while (d_iter.hasNext()) {
        QFileInfo tmp(d_iter.next());
        if (tmp.isDir() && tmp.absoluteFilePath().length() > path.length())
            folder_dropped(tmp.absoluteFilePath());
        else
            file_dropped(tmp.absoluteFilePath());
    }
}

/**
 * @brief ProjectWidget::get_index_path
 * Returns the index path for the item.
 * For a item two folders down the path could be 011 or 051.
 * @param item
 * @return index path
 */
std::stack<int> ProjectWidget::get_index_path(QTreeWidgetItem *item){
    std::stack<int> index_path;
    QTreeWidgetItem* p = &*item;
    while (p->parent() != nullptr) {
        index_path.push(p->parent()->indexOfChild(p));
        p = p->parent();
    }
    index_path.push(indexOfTopLevelItem(p));
    return index_path;
}

/**
 * @brief ProjectWidget::get_video_item
 * Searches the project tree for the VideoItem containing the VideoProjectt v_proj
 * @param v_proj    :   target VideoProject
 * @return VideoItem*   :   the correct VideoItem if found else nullptr
 */
VideoItem *ProjectWidget::get_video_item(VideoProject *v_proj, QTreeWidgetItem* s_item) {
    if (s_item == nullptr) s_item = invisibleRootItem(); // Search from the top
    for (auto i = 0; i < s_item->childCount(); ++i) {
        QTreeWidgetItem* item = s_item->child(i);
        if (item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            if (v_item->get_video_project() == v_proj) return v_item;
        } else if (item->type() == FOLDER_ITEM) {
            VideoItem* v_item = get_video_item(v_proj, item);
            if (v_item != nullptr) return v_item;
        }
    }
    return nullptr;
}

void ProjectWidget::get_video_items(QTreeWidgetItem *root, std::vector<VideoItem*>& items)
{
    if(root->type() == VIDEO_ITEM) {
        items.push_back(dynamic_cast<VideoItem*>(root));
        return;
    }
    for(int i = 0; i < root->childCount(); i++){
        QTreeWidgetItem* item = root->child(i);
        if(item->type() == VIDEO_ITEM) items.push_back(dynamic_cast<VideoItem*>(item));
        if(item->type() != VIDEO_ITEM) get_video_items(item, items);
    }
}

/**
 * @brief ProjectWidget::insert_to_path_index
 * Inserts a VideoProject into the project tree based on its index_path
 * @param vid_proj  :   VideoProject
 */
void ProjectWidget::insert_to_path_index(VideoProject *vid_proj) {
    // Index path is on form x:x:x:x..
    // Split at delimiter ':' to get index
    std::stringstream ss(vid_proj->get_index_path());
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, ':')) {
       elems.push_back(std::move(item));
    }

    // Get item at index path and set video project
    if (elems.size() > 0) {
        // Follow index path
        QTreeWidgetItem* item = topLevelItem(std::stoi(elems[0]));
        for (auto i = 1; i < elems.size(); ++i) {
            if (item->child(std::stoi(elems[i]))) item = item->child(std::stoi(elems[i]));
        }

        // Create and add VideoItem
        if (item != nullptr && item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            v_item->set_video_project(vid_proj);
            add_analyses_to_item(v_item);
        }
    }
}

/**
 * @brief ProjectWidget::update_index_paths
 * Stores GUI information of each tree item into its data member
 * @param item
 */
void ProjectWidget::save_item_data(QTreeWidgetItem* item) {
    if (item == nullptr) item = invisibleRootItem();
    for (auto i = 0; i < item->childCount(); ++i){
        auto child = item->child(i);
        if (child->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(child);
            v_item->get_video_project()->set_tree_index(get_index_path(item->child(i)));
            save_item_data(child);
        } else if (child->type() == FOLDER_ITEM) {
            save_item_data(child);
        } else if (child->type() == TAG_ITEM || child->type() == ANALYSIS_ITEM) {
            auto r_item = dynamic_cast<TreeItem*>(child);
            r_item->rename();
        }
    }
}

/**
 * @brief ProjectWidget::add_analyses_to_item
 * Adds analyses to an existing VideoItem
 * Should be used when opening a project
 * @param v_item
 */
void ProjectWidget::add_analyses_to_item(VideoItem *v_item) {
    for (std::pair<int,BasicAnalysis*> ana : v_item->get_video_project()->get_analyses()){
        if (ana.second->get_type() == TAG) {
            TagItem* tag_item = new TagItem(dynamic_cast<Tag*>(ana.second));
            v_item->addChild(tag_item);
        } else {
            AnalysisItem* ana_item = new AnalysisItem(dynamic_cast<AnalysisProxy*>(ana.second));
            v_item->addChild(ana_item);
        }
    }
}

/**
 * @brief ProjectWidget::dragEnterEvent
 * Makes sure the ProjectWidget only accepts drops containing urls or objects of the correct mime type
 * @param event
 */
void ProjectWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls() && m_proj != nullptr) {
        // Files
        event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")){
        // TreeItem
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

/**
 * @brief ProjectWidget::dropEvent
 * Handels drop events. Will try to add files if they are dropped.
 * Calls upon standard dropEvent for TreeItems.
 * @param event
 */
void ProjectWidget::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        // Files
        for (auto &url : event->mimeData()->urls()) {
            QString video_path = url.toLocalFile();
            QFileInfo f_info(video_path);
            if (f_info.isDir()) {
                folder_dropped(video_path);
            } else {
                file_dropped(video_path);
            }
        }
    } else {
        QList<QTreeWidgetItem*> items = selectedItems();
        QTreeWidget::dropEvent(event);
        // Update index paths
        for (auto item : items) {
            if (item->type() == VIDEO_ITEM) {
                auto vid_item = dynamic_cast<VideoItem*>(item);
                vid_item->get_video_project()->set_tree_index(get_index_path(item));
            }
        }
    }
}

void ProjectWidget::advanced_analysis()
{
    std::vector<VideoItem*> v_items;
    QTreeWidgetItem* s_item = invisibleRootItem();
    get_video_items(s_item, v_items);
    if(v_items.empty()) return;
    AnalysisDialog* dialog = new AnalysisDialog(v_items,m_proj->getDir());
    connect(dialog, &AnalysisDialog::start_analysis, this, &ProjectWidget::advanced_analysis_setup);
    dialog->show();
}


void ProjectWidget::advanced_analysis_setup(AnalysisMethod * method, VideoProject* vid_proj)
{
    if (vid_proj == nullptr) return;
    VideoItem* v_item = get_video_item(vid_proj);
    AnalysisItem* ana = new AnalysisItem();
    v_item->addChild(ana);
    ana->setText(0, "Loading");
    v_item->setExpanded(true);
    emit begin_analysis(dynamic_cast<QTreeWidgetItem*>(ana), method);
}

/**
 * @brief ProjectWidget::prompt_save
 * Prompts the user for a save before continuing.
 * Returns false on cancel
 */
void ProjectWidget::prompt_save() {
    QMessageBox delete_box(this);
    delete_box.setIcon(QMessageBox::Warning);
    delete_box.setText("There are unsaved changes.\n");
    delete_box.setInformativeText("Do you wish to save before closing the project?");
    delete_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    delete_box.setDefaultButton(QMessageBox::No);
    if (delete_box.exec() == QMessageBox::Yes) {
        save_project();
    }
}


/**
 * @brief ProjectWidget::tree_item_clicked
 * Slot function for when a tree item is clicked.
 * Preforms different operations based on tree item type.
 * @param item
 * @param col
 */
void ProjectWidget::tree_item_clicked(QTreeWidgetItem* item, const int& col) {
    get_index_path(item);
    switch(item->type()){
    case VIDEO_ITEM: {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        emit marked_video(vid_item->get_video_project());
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(false);
        emit enable_poi_btns(false,false);
        emit enable_tag_btn(false);
        emit update_frame();
        break;
    } case ANALYSIS_ITEM: {
        tree_item_clicked(item->parent());
        AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(item);
        if(!ana_item->is_finished()) break;
        emit marked_analysis(ana_item->get_analysis());
        emit marked_basic_analysis(dynamic_cast<BasicAnalysis*>(ana_item->get_analysis()));
        emit set_detections(true);
        emit set_poi_slider(true);
        emit enable_poi_btns(true, true);
        emit update_frame();
        break;
    } case TAG_ITEM: {
        tree_item_clicked(item->parent());
        TagItem* tag_item = dynamic_cast<TagItem*>(item);
        emit marked_basic_analysis(tag_item->get_tag());
        emit set_tag_slider(true);
        emit enable_poi_btns(true, false);
        emit enable_tag_btn(true);
        emit update_frame();
        break;
    } case FOLDER_ITEM: {
        break;
    } default:
        break;
    }
}

/**
 * @brief ProjectWidget::check_selection
 * Deselects all items that doesn't have selection_parent as parent
 */
void ProjectWidget::check_selection(){
    if (selectedItems().count() <= 1) return;
    this->blockSignals(true);
    for (auto i : selectedItems()) {
        if (i->parent() != selection_parent) i->setSelected(false);
    }
    this->blockSignals(false);
}

/**
 * @brief ProjectWidget::check_selection_level
 * Sets the parent of the current selection if the selection is of size one.
 */
void ProjectWidget::check_selection_level(QTreeWidgetItem*, QTreeWidgetItem*) {
    if (selectedItems().count() == 1) selection_parent = selectedItems().front()->parent();
}

/**
 * @brief ProjectWidget::context_menu
 * Slot function triggered by customContextMenuRequested
 * Creates a context menu
 * @param point :   click position
 */
void ProjectWidget::context_menu(const QPoint &point) {
    if (m_proj == nullptr) return;
    QMenu menu(this);

    const int item_count = selectedItems().count();
    if (item_count == 0) {
        // Clicked on root tree
        menu.addAction("New Folder", this, SLOT(create_folder_item()));
    } else if (item_count == 1) {
        // Clicked on item
        menu.addAction("New Folder", this, SLOT(create_folder_item()));
        menu.addSeparator();

        switch (selectedItems().front()->type()) {
            case TAG_ITEM:
            case ANALYSIS_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                break;
            case FOLDER_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                menu.addAction("Remove", this, SLOT(remove_item()));
                break;
            case VIDEO_ITEM:
                menu.addAction("Remove", this, SLOT(remove_item()));
            default:
                // VIDEO_ITEM
                break;
        }
    } else if (item_count > 1) {
        // Clicked whilst multiple items were selected
        menu.addAction("Remove", this, SLOT(remove_item()));
    }
    menu.exec(mapToGlobal(point));
}


/**
 * @brief ProjectWidget::remove_item
 * Deletes all selected items.
 * If a folder is selected then it will delete all subitems as well.
 */
void ProjectWidget::remove_item() {
    QMessageBox delete_box(this);
    delete_box.setIcon(QMessageBox::Warning);
    delete_box.setText("Deleting item(s)\n"
                       "(Unselected items within selected folders will be deleted as well)");
    delete_box.setInformativeText("Do you wish to continue?");
    delete_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    delete_box.setDefaultButton(QMessageBox::No);
    if (delete_box.exec() == QMessageBox::Yes) {
        for (auto item : selectedItems()) {
            delete item;
        }
    }
    emit remove_overlay();

}

/**
 * @brief ProjectWidget::rename_item
 * Renames the currentItem in the tree
 */
void ProjectWidget::rename_item(){
    editItem(currentItem());
}

/**
 * @brief ProjectWidget::create_folder_item
 * Creates a new folder in the project tree
 */
void ProjectWidget::create_folder_item() {
    if (m_proj == nullptr) return;
    FolderItem* item = new FolderItem();
    item->setText(0, tr("New Folder"));
    QTreeWidgetItem* s_item = (!selectedItems().count()) ? invisibleRootItem() : currentItem();
    if (s_item == invisibleRootItem()) {
        // Click occured on background add to top level
        insertTopLevelItem(topLevelItemCount(), item);
    } else if (s_item->type() == FOLDER_ITEM) {
        // Clicked on folder item. Add new folder as child
        s_item->insertChild(0, item);
        s_item->setExpanded(true);
    } else if (s_item->type() == VIDEO_ITEM) {
        QTreeWidgetItem* p_item =  s_item->parent();
        if (p_item == nullptr) {
            insertTopLevelItem(indexOfTopLevelItem(s_item) + 1, item);
        } else {
            int index = p_item->indexOfChild(s_item);
            p_item->insertChild(index + 1, item);
        }
    }
    editItem(item);
    clearSelection();
    item->setSelected(true);
}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
void ProjectWidget::save_project() {
    if (m_proj == nullptr ) return;
    save_item_data();
    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.save_state(m_proj->getDir() + "treestate");
    m_proj->save_project();
    RecentProject rp;
    rp.load_recent();
    rp.update_recent(m_proj->getName(), m_proj->full_path());
    set_status_bar("Project saved");
}

/**
 * @brief ProjectWidget::open_project
 * Slot function to open a previously created project
 */
void ProjectWidget::open_project(QString project_path) {
    if (project_path.isEmpty()) return;
    if (m_proj != nullptr) close_project();
    set_status_bar("Opening project");

    m_proj = Project::fromFile(project_path.toStdString());
    // Load project tree structure
    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.load_state(m_proj->getDir() + "treestate");

    parentWidget()->parentWidget()->setWindowTitle(QString::fromStdString(m_proj->getName()));
    emit proj_path(m_proj->getDir());
    for (auto vid_proj : m_proj->get_videos()) {
        insert_to_path_index(vid_proj);
        emit load_bookmarks(vid_proj);
    }
}

/**
 * @brief ProjectWidget::close_project
 * Closes the current project if there is one
 */
void ProjectWidget::close_project() {
    // TODO Check for unsaved changes before closing
    // Prompt: There are unsaved changes in the project. Do you wish to save before continuing?
    if (m_proj == nullptr) return;

    bool cont = false;
    if (!m_proj->is_saved()){
        prompt_save();
    }
    emit set_status_bar("Closing project");
    emit project_closed();
    emit remove_overlay();
    this->clear();
    delete m_proj;
    m_proj = nullptr;
}

/**
 * @brief ProjectWidget::remove_project
 * Removes the current project if there is one
 */
void ProjectWidget::remove_project() {
    // TODO Does this delete all images?
    if (m_proj == nullptr) return;
    QMessageBox msg_box;
    msg_box.setText("Are you sure you want to remove the project?");
    msg_box.setInformativeText("This will delete all project files (images, reports, etc).");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();

    if (reply != QMessageBox::Yes) return;
    emit set_status_bar("Removing project and associated files");

    m_proj->delete_artifacts();
    this->clear();
    delete m_proj;
    m_proj = nullptr;
    emit project_closed();
}
