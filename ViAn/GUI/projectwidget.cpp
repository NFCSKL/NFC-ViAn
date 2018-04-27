#include "projectwidget.h"
#include "projectdialog.h"
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

    connect(this, &ProjectWidget::itemChanged, this, &ProjectWidget::update_item_data);
}

ProjectWidget::~ProjectWidget() {
    delete m_proj;
}


/**
 * @brief ProjectWidget::new_project
 * Creates a new empty project if current project is closed/user closes it
 */
void ProjectWidget::new_project() {
//    ProjectDialog* proj_dialog = new ProjectDialog();
//    QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
//    QObject::connect(proj_dialog, SIGNAL(open_project(QString)), this, SLOT(open_project(QString)));
    add_project("New project", "");

}

/**
 * @brief ProjectWidget::add_project
 * Slot function called by the create project dialog.
 * Creates a new project and generates the default tree structure
 * @param project_name
 * @param project_path
 */
void ProjectWidget::add_project(QString project_name, QString project_path) {
    if (!close_project()) return;

    std::string name = project_name.toStdString();
    std::string path = project_path.toStdString();
    Project* new_proj = new Project(name, path);
    set_main_window_name(project_name);
    m_proj = new_proj;
    path.append(name);
    emit proj_path(m_proj->get_dir());
}

/**
 * @brief ProjectWidget::add_video
 * Slot function to add a video.
 * Creates a file dialog and creates a video project based on file path
 */
void ProjectWidget::add_video() {
    if (m_proj == nullptr)  return;

    //Build string to limit file selection
    QString extensions = "Videos (";
    for (auto it = allowed_vid_exts.begin(); it != allowed_vid_exts.end(); ++it){
        extensions += "*." + QString::fromStdString(*it) + " ";
    }
    extensions += ")";

    // Create actual dialog
    QStringList video_paths = QFileDialog().getOpenFileNames(
                this,
                tr("Add video"),
                m_proj->get_dir().c_str(),
                extensions);

    for (auto video_path : video_paths){
        int index = video_path.lastIndexOf('/') + 1;
        QString vid_name = video_path.right(video_path.length() - index);
        // TODO Check if file is already added
        VideoProject* vid_proj = new VideoProject(new Video(video_path.toStdString()));
        qDebug() << "added a new one";
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
    AnalysisMethod* method = new MotionDetection(vid_proj->get_video()->file_path, m_proj->m_dir);
    if(settings->use_bounding_box) method->setBounding_box(settings->bounding_box);
    if(settings->use_interval) method->set_interval(settings->get_interval());

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
    vid_proj->add_analysis(tag);
    VideoItem* vid_item = get_video_item(vid_proj);
    if (vid_item == nullptr) {
        set_status_bar("Something went wrong when adding tag: " + QString::fromStdString(tag->get_name()));
        return;
    }

    if (tag->get_type() == DRAWING_TAG) {
        DrawingTagItem* item = new DrawingTagItem(dynamic_cast<DrawingTag*>(tag));
        vid_item->addChild(item);
        clearSelection();
        item->setSelected(true);
        tree_item_clicked(item);
    } else if (tag->get_type() == TAG) {
        TagItem* item = new TagItem(dynamic_cast<Tag*>(tag));
        vid_item->addChild(item);
        clearSelection();
        item->setSelected(true);
        tree_item_clicked(item);
    }
    vid_item->setExpanded(true);
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
    VideoItem* vid_item = new VideoItem(vid_proj);

    // If there only is one selected item and it's a folder,
    // add the new video to that folder otherwise to the top level
    QTreeWidgetItem* s_item = (!selectedItems().count()) ? invisibleRootItem() : currentItem();
    if (s_item->type() == FOLDER_ITEM) {
        s_item->addChild(vid_item);
        s_item->setExpanded(true);
    } else {
        insertTopLevelItem(topLevelItemCount(), vid_item);
    }
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
    QFileInfo tmp(path);
    std::string ext = tmp.suffix().toStdString();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (allowed_vid_exts.find(ext) != allowed_vid_exts.end()) {
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
 * Searches the project tree for the VideoItem containing the VideoProject v_proj
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

void ProjectWidget::get_video_items(QTreeWidgetItem *root, std::vector<VideoItem*>& items) {
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
        } else if (child->type() == TAG_ITEM || child->type() == ANALYSIS_ITEM || child->type() == DRAWING_TAG_ITEM) {
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
        } else if (ana.second->get_type() == DRAWING_TAG) {
            DrawingTagItem* tag_item = new DrawingTagItem(dynamic_cast<DrawingTag*>(ana.second));
            v_item->addChild(tag_item);
        } else if (ana.second->get_type() == MOTION_DETECTION) {
            AnalysisItem* ana_item = new AnalysisItem(dynamic_cast<AnalysisProxy*>(ana.second));
            v_item->addChild(ana_item);
        } else {
            qWarning() << "Something went wrong while adding analyses to items.";
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
            } else if (item->type() == FOLDER_ITEM) {
                m_proj->set_unsaved(true);
            }
        }
    }
}

void ProjectWidget::advanced_analysis() {
    std::vector<VideoItem*> v_items;
    QTreeWidgetItem* s_item = invisibleRootItem();
    get_video_items(s_item, v_items);
    if(v_items.empty()) return;
    AnalysisDialog* dialog = new AnalysisDialog(v_items, m_proj->get_dir());
    connect(dialog, &AnalysisDialog::start_analysis, this, &ProjectWidget::advanced_analysis_setup);
    dialog->show();
}


void ProjectWidget::advanced_analysis_setup(AnalysisMethod * method, VideoProject* vid_proj) {
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
 * Prompts the user for to save before continuing.
 * Returns false on cancel
 */
bool ProjectWidget::prompt_save() {
    bool ok = true;

    QMessageBox delete_box(this);
    delete_box.setIcon(QMessageBox::Warning);
    delete_box.setText("There are unsaved changes.");
    delete_box.setInformativeText("Do you wish to save before continuing?");
    delete_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    delete_box.setDefaultButton(QMessageBox::Yes);

    switch (delete_box.exec()){
        case QMessageBox::Yes:
                ok = save_project();
                break;
        case QMessageBox::Cancel:
                ok = false;
                break;
    }
    return ok;
}

/**
 * @brief ProjectWidget::tree_item_clicked
 * Slot function for when a tree item is clicked.
 * Performs different operations based on tree item type.
 * @param item
 * @param col
 */
void ProjectWidget::tree_item_clicked(QTreeWidgetItem* item, const int& col) {
    Q_UNUSED(col)
    if (!item) return;
    //get_index_path(item); // Remove?
    switch(item->type()){
    case VIDEO_ITEM: {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        emit marked_video(vid_item->get_video_project(), -1);
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
    } case DRAWING_TAG_ITEM: {
        tree_item_clicked(item->parent());
        DrawingTagItem* tag_item = dynamic_cast<DrawingTagItem*>(item);
        emit marked_basic_analysis(tag_item->get_tag());
        emit set_tag_slider(true);
        emit enable_poi_btns(true, false);
        emit enable_tag_btn(true);
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
 * @brief ProjectWidget::update_item_data
 * Updates the underlying data structures of the tree items
 * @param item:     item that has been changed
 * @param column    column in item
 */
void ProjectWidget::update_item_data(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);

    std::string item_text = item->text(0).toStdString();
    switch(item->type()) {
        case TAG_ITEM: {
            auto tag_item = dynamic_cast<TagItem*>(item);
            Tag* tag = tag_item->get_tag();
            if (tag->get_name() != item_text) tag->set_name(item_text);
            break;
        } case ANALYSIS_ITEM: {
            auto analysis_item = dynamic_cast<AnalysisItem*>(item);
            AnalysisProxy* analysis = analysis_item->get_analysis();
            // AnalysisItems are added when an analysis is started
            // and the proxy can thusly be a nullptr
            if (analysis == nullptr) break;
            if (analysis->get_name() != item_text) analysis->set_name(item_text);
            break;
        } case FOLDER_ITEM: {
            m_proj->set_unsaved(true);
        }
    }
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
        QTreeWidgetItem* item = selectedItems().front();
        switch (item->type()) {
            case TAG_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                menu.addAction("Remove", this, SLOT(remove_item()));
                break;
            case DRAWING_TAG_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                menu.addAction("Update", this, SLOT(drawing_tag()));
                menu.addAction("Remove", this, SLOT(remove_item()));
                break;
            case ANALYSIS_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                menu.addAction("Show details", this, SLOT(show_details()));
                menu.addAction("Hide details", this, SLOT(hide_details()));
                menu.addAction("Remove", this, SLOT(remove_item()));
                break;
            case FOLDER_ITEM:
                menu.addAction("Rename", this, SLOT(rename_item()));
                menu.addAction("Remove", this, SLOT(remove_item()));
                break;
            case VIDEO_ITEM:
                menu.addAction("Remove", this, SLOT(remove_item()));
                menu.addAction("Tag drawings", this, SLOT(drawing_tag()));
                break;
            default:
                break;
        }
    } else if (item_count > 1) {
        // Clicked whilst multiple items were selected
        menu.addAction("Remove", this, SLOT(remove_item()));
    }
    menu.exec(mapToGlobal(point));
}

/**
 * @brief ProjectWidget::drawing_tag
 * Creates the drawing tag or updates the current one depending
 * on the current QTreeItem.
 * The drawing tag will tag all frames which have a drawing on them.
 */
void ProjectWidget::drawing_tag() {
    VideoItem* vid_item;
    DrawingTag* tag;
    if (selectedItems().front()->type() == VIDEO_ITEM) {
        // Create tag drawing
        vid_item = dynamic_cast<VideoItem*>(selectedItems().front());
        tag = new DrawingTag();
        tag->m_name = "Drawing tag";
    } else if (selectedItems().front()->type() == DRAWING_TAG_ITEM) {
        // Update tag drawing
        DrawingTagItem* item = dynamic_cast<DrawingTagItem*>(selectedItems().front());
        vid_item = dynamic_cast<VideoItem*>(item->parent());
        tag = item->get_tag();
        tag->clear_intervals();
    }

    VideoProject* vid_proj = vid_item->get_video_project();
    for (auto const& frame_overlay : vid_proj->get_overlay()->get_overlays()) {
        if (frame_overlay.second.size() > 0) {
            qDebug() << "Tag frame" << frame_overlay.first;
            tag->add_frame(frame_overlay.first);
        }
    }

    if (selectedItems().front()->type() == VIDEO_ITEM) {
        qDebug() << "Add the drawing tag";
        add_basic_analysis(vid_proj, tag);
    } else if (selectedItems().front()->type() == DRAWING_TAG_ITEM) {
        tree_item_clicked(dynamic_cast<DrawingTagItem*>(selectedItems().front()));
    }
}

/**
 * @brief ProjectWidget::show_details
 * @param ana_item
 * Show the analysis' details; interval and bounding box
 */
void ProjectWidget::show_details() {
    emit show_analysis_details(true);
}

/**
 * @brief ProjectWidget::hide_details
 * Hide the analysis' details; interval and bounding box
 */
void ProjectWidget::hide_details() {
    emit show_analysis_details(false);
}


/**
 * @brief ProjectWidget::remove_item
 * Deletes all selected items.
 * If a folder is selected then it will delete all subitems as well.
 */
void ProjectWidget::remove_item() {
    QString text = "Deleting item(s)\n"
                   "(Unselected items within selected folders will be deleted as well)";
    QString info_text = "Do you wish to continue?";
    if (message_box(text, info_text, true)) {
        for (auto item : selectedItems()) {
            remove_tree_item(item);
        }
    }
}

/**
 * @brief ProjectWidget::remove_tree_item
 * @param item
 * Recursive function for removing item and all of its children
 */
void ProjectWidget::remove_tree_item(QTreeWidgetItem* item) {
    if (item->type() == FOLDER_ITEM) {
        while (item->childCount() != 0) {
            remove_tree_item(item->child(0));
        }
    }
    else if (item->type() == VIDEO_ITEM) {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        VideoProject* v_proj = vid_item->get_video_project();

        // Remove all children
        while (item->childCount() != 0) {
            remove_tree_item(item->child(0));
        }
        // Remove the video from the list of videos
        qDebug() << m_proj->get_videos().size() << "size1";
        auto it = std::find(m_proj->get_videos().begin(), m_proj->get_videos().end(), v_proj);
        if (it != m_proj->get_videos().end()) {
            m_proj->get_videos().erase(it);
        }
        qDebug() << m_proj->get_videos().size() << "size2";
        delete v_proj;
        emit item_removed(v_proj);
        emit remove_overlay();
    }
    else if (item->type() == TAG_ITEM) {
        emit set_tag_slider(false);
        emit enable_tag_btn(false);

        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        Tag* tag = dynamic_cast<TagItem*>(item)->get_tag();
        vid_item->get_video_project()->remove_analysis(tag);
        emit update_frame();
    }
    else if (item->type() == DRAWING_TAG_ITEM) {
        emit set_tag_slider(false);
        emit enable_tag_btn(false);

        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        DrawingTag* tag = dynamic_cast<DrawingTagItem*>(item)->get_tag();
        vid_item->get_video_project()->remove_analysis(tag);
        emit update_frame();
    }
    else if (item->type() == ANALYSIS_ITEM) {
        emit set_detections(false);
        emit set_poi_slider(false);
        emit enable_poi_btns(false, false);

        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        AnalysisProxy* analysis = dynamic_cast<AnalysisItem*>(item)->get_analysis();

        // TODO Add this line to remove the analysis from the folder
        analysis->delete_saveable(analysis->full_path());
        vid_item->get_video_project()->remove_analysis(analysis);
        emit update_frame();
        emit clear_analysis();
    }
    delete item;
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
    } else if (s_item->type() == TAG_ITEM || s_item->type() == DRAWING_TAG_ITEM || s_item->type() == ANALYSIS_ITEM) {
        QTreeWidgetItem* p_item = s_item->parent()->parent();
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
    m_proj->set_unsaved(true);
}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
bool ProjectWidget::save_project() {
    if (m_proj == nullptr ) return false;

    // Move all project files if the current project is temporary
    // i.e. has not been saved yet
    if (m_proj->is_temporary()) {
        QString name{}, path{};
//        std::unique_ptr<ProjectDialog> project_dialog(new ProjectDialog(&name, &path));
        ProjectDialog* project_dialog = new ProjectDialog(&name, &path, this);
        connect(project_dialog, &ProjectDialog::open_project, this, &ProjectWidget::open_project);
        int status = project_dialog->exec();

        if (status == project_dialog->Accepted) {
            // User clicked ok, dialog checked for proper path & name
            // Update project path
            // TODO: Update window title to new project name
            m_proj->copy_directory_files(QString::fromStdString(m_proj->get_dir()), path + name, true, std::vector<std::string>{"vian"});
            m_proj->remove_files();
            m_proj->set_name_and_path(name.toStdString(), path.toStdString());
            m_proj->set_temporary(false);
            set_main_window_name(name);
            emit proj_path(m_proj->get_dir());
            QDir dir;
            dir.mkpath(QString::fromStdString(m_proj->get_dir()));
        } else {
            // User aborted dialog, cancel save
            return false;
        }

    }

    save_item_data();
    emit save_draw_wgt();

    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.save_state(m_proj->get_dir() + "treestate");

    m_proj->save_project();

    RecentProject rp;
    rp.load_recent();
    rp.update_recent(m_proj->get_name(), m_proj->get_file());
    set_status_bar("Project saved");
    return true;
}

/**
 * @brief ProjectWidget::open_project
 * Slot function to open a previously created project
 * Returns true if the project has been opened.
 */
bool ProjectWidget::open_project(QString project_path) {
    if (project_path.isEmpty()) return false;

    if (m_proj && !close_project())
        return false;

    set_status_bar("Opening project");
    auto new_proj = Project::fromFile(project_path.toStdString());
    if (new_proj == nullptr) {
        qWarning() << "Something went wrong while creating the temporary folder.";
        return false;
    }
    m_proj = new_proj;
    set_status_bar("Opening project");
\
    // Load project tree structure
    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.load_state(m_proj->get_dir() + "treestate");
    set_main_window_name(QString::fromStdString(m_proj->get_name()));
    emit proj_path(m_proj->get_dir());
    for (auto vid_proj : m_proj->get_videos()) {
        insert_to_path_index(vid_proj);
        emit load_bookmarks(vid_proj);
    }
    return true;
}

/**
 * @brief ProjectWidget::close_project
 * Closes the current project if there is one and user accepts prompt
 * Returns true if the project has been closed.
 */
bool ProjectWidget::close_project() {
    qDebug() << "Close project";
    if (m_proj == nullptr) return true;

    // Prompt user to save. If cancel keep the current project
    bool abort_close = false;
    if (!m_proj->is_saved()){
        abort_close = !prompt_save();
        if (abort_close) {
            return false;
        }
    }

    // Remove project if temporary
    if (m_proj->is_temporary()) {
        m_proj->remove_files();
    }

    emit set_status_bar("Closing project");
    emit project_closed();
    emit remove_overlay();

    delete m_proj;
    m_proj = nullptr;
    this->clear();
    return true;
}

/**
 * @brief ProjectWidget::remove_project
 * Removes the current project if there is one
 */
void ProjectWidget::remove_project() {
    // TODO Does this delete all images?
    if (m_proj == nullptr) return;
    QString text = "Are you sure you want to remove the project?";
    QString info_text = "This will delete all project files (images, reports, etc).";
    if (!message_box(text, info_text)) return;
  
    set_main_window_name(QString::fromStdString(""));
    emit set_status_bar("Removing project and associated files");

    m_proj->remove_files();
    this->clear();
    delete m_proj;
    m_proj = nullptr;
    //emit remove_overlay();
    emit project_closed();
    emit remove_overlay();
    new_project();
}

void ProjectWidget::set_main_window_name(QString name) {
    parentWidget()->parentWidget()->setWindowTitle("ViAn  -  " + name);
}

/**
 * @brief ProjectWidget::message_box
 * Creates a popup yes/no message box with the inputed text
 * @param text  :   Text
 * @param info_text :   Informative text
 * @param warning   :   true will make the message box a warning box
 * @return Yes returns true and no returns false
 */
bool ProjectWidget::message_box(QString text, QString info_text, bool warning) {
    QMessageBox msg_box;
    if (warning) msg_box.setIcon(QMessageBox::Warning);
    msg_box.setText(text);
    msg_box.setInformativeText(info_text);
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();
    return reply == QMessageBox::Yes;
}
