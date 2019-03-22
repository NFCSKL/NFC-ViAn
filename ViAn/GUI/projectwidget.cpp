#include "projectwidget.h"

#include "Analysis/analysisdialog.h"
#include "Analysis/motiondetection.h"
#include "constants.h"
#include "GUI/TreeItems/analysisitem.h"
#include "GUI/TreeItems/drawingtagitem.h"
#include "GUI/TreeItems/folderitem.h"
#include "GUI/TreeItems/intervalareaitem.h"
#include "GUI/TreeItems/intervalitem.h"
#include "GUI/TreeItems/sequenceitem.h"
#include "GUI/TreeItems/tagframeitem.h"
#include "GUI/TreeItems/tagitem.h"
#include "GUI/TreeItems/videoitem.h"
#include "imageimporter.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/interval.h"
#include "Project/Analysis/tag.h"
#include "Project/Analysis/tagframe.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/projecttreestate.h"
#include "Project/recentproject.h"
#include "Project/videoproject.h"
#include "projectdialog.h"
#include "sequencedialog.h"
#include "utility.h"
#include "viewpathdialog.h"


#include <QAction>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QProgressDialog>
#include <QShortcut>
#include <QThread>

#include <sstream>
#include <string>
#include <vector>

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

    // Create togglable action in the context menu for analysis details
    show_details_act = new QAction("Show/hide anlaysis details", this);
    show_details_act->setCheckable(true);
    connect(show_details_act, &QAction::triggered, this, &ProjectWidget::toggle_analysis_details);

    // Create togglable action in the context menu for analysis settings
    show_settings_act = new QAction("Show/hide analysis info", this);
    show_settings_act->setCheckable(true);
    connect(show_settings_act, &QAction::triggered, this, &ProjectWidget::toggle_settings_details);

    connect(this, &ProjectWidget::customContextMenuRequested, this, &ProjectWidget::context_menu);
    connect(this, &ProjectWidget::currentItemChanged, this, &ProjectWidget::tree_item_changed);

    // Widget only shortcut for creating a new folder
    QShortcut* new_folder_sc = new QShortcut(this);
    new_folder_sc->setContext(Qt::WidgetWithChildrenShortcut);
    new_folder_sc->setKey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    connect(new_folder_sc, &QShortcut::activated, this, &ProjectWidget::create_folder_item);

    // Shortcut for deleting item
    QShortcut* delete_sc = new QShortcut(QKeySequence::Delete, this);
    delete_sc->setContext(Qt::WidgetWithChildrenShortcut);
    connect(delete_sc, &QShortcut::activated, this, &ProjectWidget::remove_item);

    // Shortcut for updating drawing tag item
    QShortcut* update_sc = new QShortcut(QKeySequence(Qt::Key_F5), this);
    update_sc->setContext(Qt::WidgetShortcut);
    connect(update_sc, &QShortcut::activated, this, &ProjectWidget::drawing_tag);

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
    add_project("New project", "");
}

/**
 * @brief ProjectWidget::add_project
 * Slot function called by the create project dialog.
 * Creates a new project and generates the default tree structure
 * @param project_name
 * @param project_path
 */
void ProjectWidget::add_project(const QString project_name, const QString project_path) {
    if (!close_project()) return;

    Project* new_proj = new Project(project_name, project_path);
    set_main_window_name(project_name);
    m_proj = new_proj;
    emit set_project(m_proj);
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
        extensions += "*." + *it + " ";
    }
    extensions += ")";

    // Create actual dialog
    QStringList video_paths = QFileDialog().getOpenFileNames(
                this,
                tr("Add video"),
                m_proj->get_dir(),
                extensions);

    for (auto video_path : video_paths){
        create_video(video_path);
    }
}

/**
 * @brief ProjectWidget::create_video
 * Create the video object and add it to the tree
 * @param path
 */
void ProjectWidget::create_video(QString path) {
    int index = path.lastIndexOf('/') + 1;
    QString vid_name = path.right(path.length() - index);
    // TODO Check if file is already added
    Video* video = new Video(path);
    VideoProject* vid_proj = new VideoProject(video);
    m_proj->add_video_project(vid_proj);
    tree_add_video(vid_proj, vid_name);
    video_list.push_back(video);
}


/**
 * @brief ProjectWidget::generate_video
 * @param path
 * Add the video generated from the videoedit widget to the tree
 * this video will be added in a folder called "Generated videos"
 * this folder will be created if it does not already exist
 */
void ProjectWidget::generate_video(QString path) {
    QString folder_name = "Generated videos";
    FolderItem* folder_item = nullptr;
    for(int i = 0; i < invisibleRootItem()->childCount(); ++i) {
        QTreeWidgetItem* item = invisibleRootItem()->child(i);
        if (item->type() == FOLDER_ITEM) {
            if (item->text(0) == folder_name) {
                folder_item = dynamic_cast<FolderItem*>(item);
                break;
            }
        }
    }
    if (folder_item == nullptr) {
        folder_item = new FolderItem();
        folder_item->setText(0, folder_name);
        addTopLevelItem(folder_item);
    }

    // Add the video to the tree
    int index = path.lastIndexOf('/') + 1;
    QString vid_name = path.right(path.length() - index);
    Video* video = new Video(path);
    VideoProject* vid_proj = new VideoProject(video);
    m_proj->add_video_project(vid_proj);

    VideoItem* vid_item = new VideoItem(vid_proj);
    vid_item->setToolTip(0, vid_proj->get_video()->file_path);
    folder_item->addChild(vid_item);
    folder_item->setExpanded(true);
    vid_proj->set_tree_index(get_index_path(dynamic_cast<QTreeWidgetItem*>(vid_item)));
    video_list.push_back(video);
}

/**
 * @brief ProjectWidget::add_images
 * Slot function for adding image sequences
 * Each selected image is copied to the project folder
 */
void ProjectWidget::add_images() {
    if (m_proj == nullptr)  return;

    QStringList image_paths = QFileDialog().getOpenFileNames(
                this,
                tr("Add images"),
                m_proj->get_dir());

    // Assert that user selected files
    if (!image_paths.size()){
        return;
    }

    QString text{};
    int sequence_type;
    SequenceDialog* seq_dialog = new SequenceDialog(&text, &sequence_type, this);

    bool reply = seq_dialog->exec();

    // Check if dialog was accepted and that proper name was used
    QString seq_name{"sequence"};
    if (reply && !text.isEmpty()) {
        seq_name = text;
    } else {
        return;
    }

    QString path = m_proj->get_dir() + "Sequences/" + seq_name;

    QProgressDialog* progress = new QProgressDialog(
                "Copying images...", "Abort", 0, image_paths.size(), this, Qt::WindowMinimizeButtonHint);
    ImageImporter* importer = new ImageImporter(image_paths, path, sequence_type);
    QThread* copy_thread = new QThread();
    importer->moveToThread(copy_thread);

    connect(progress, &QProgressDialog::canceled, importer, &ImageImporter::abort);
    connect(importer, &ImageImporter::update_progress, progress, &QProgressDialog::setValue);

    connect(copy_thread, &QThread::started, importer, &ImageImporter::import_images);
    connect(importer, &ImageImporter::finished, copy_thread, &QThread::quit);
    connect(importer, &ImageImporter::finished, importer, &ImageImporter::deleteLater);
    connect(copy_thread, &QThread::finished, copy_thread, &QThread::deleteLater);
    connect(importer, &ImageImporter::imported_sequence, this, &ProjectWidget::create_sequence);
    progress->show();
    copy_thread->start();
}

void ProjectWidget::create_sequence(QStringList image_paths, QStringList checksums, QString path, int seq_type) {
    std::vector<QString> images, hashes;
    for (auto i = 0; i < image_paths.size(); i++) {
        images.push_back(image_paths.at(i));
        hashes.push_back(checksums.at(i));
    }
    VIDEO_TYPE type = static_cast<VIDEO_TYPE>(seq_type);
    VideoProject* vid_proj = new VideoProject(new ImageSequence(path, images, hashes, type));
    m_proj->add_video_project(vid_proj);
    tree_add_video(vid_proj, "test");
}

/**
 * @brief ProjectWidget::start_analysis
 * @param vid_proj
 * Start analysis on the selected video
 */
void ProjectWidget::start_analysis(VideoProject* vid_proj, AnalysisSettings* settings) {
    AnalysisMethod* method = nullptr;
    switch (settings->get_type()) {
    case MOTION_DETECTION: {
        std::string path = vid_proj->get_video()->file_path.toStdString();
        std::string dir = m_proj->m_dir.toStdString();
        method = new MotionDetection(path, dir, settings);
        break;
    }
    default:
        break;
    }
    if (settings->quick_analysis) {
        settings->set_full_settings(analysis_settings->get_full_settings());
    } else {}

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
void ProjectWidget::add_tag(VideoProject* vid_proj, Tag* tag) {
    vid_proj->add_analysis(tag);
    VideoItem* vid_item = get_video_item(vid_proj);
    if (vid_item == nullptr) {
        set_status_bar("Something went wrong when adding tag: " + tag->get_name());
        return;
    }

    if (tag->get_type() == DRAWING_TAG) {
        DrawingTagItem* item = new DrawingTagItem(tag);
        vid_item->addChild(item);
        clearSelection();
        item->setSelected(true);
        setCurrentItem(item);
        for (auto t_frame : tag->tag_map) {
            TagFrameItem* tf_item = new TagFrameItem(t_frame.second);
            item->addChild(tf_item);
        }
        item->setExpanded(true);
        tree_item_changed(item);
    } else if (tag->get_type() == TAG) {
        TagItem* item = new TagItem(tag);
        vid_item->addChild(item);
        clearSelection();
        item->setSelected(true);
        setCurrentItem(item);
        tree_item_changed(item);
    }
    vid_item->setExpanded(true);
}

/**
 * @brief ProjectWidget::add_frames_to_tag_item
 * Create the TagFrameItems from all the frames. Used when opening a project
 * @param item
 */
void ProjectWidget::add_frames_to_tag_item(TreeItem* item) {
    Tag* tag = nullptr;
    if (item->type() == TAG_ITEM) {
        tag = dynamic_cast<TagItem*>(item)->get_tag();
    } else if (item->type() == DRAWING_TAG_ITEM) {
        tag = dynamic_cast<DrawingTagItem*>(item)->get_tag();
    }
    for (auto t_frame : tag->tag_map) {
        TagFrameItem* tf_item = new TagFrameItem(t_frame.second);
        item->addChild(tf_item);
    }
}

/**
 * @brief ProjectWidget::add_new_frame_to_tag_item
 * Create a new TagFrameItem for the newly tagged frame
 * @param frame
 * @param t_frame
 */
void ProjectWidget::add_new_frame_to_tag_item(int frame, TagFrame* t_frame) {
    TagFrameItem* tf_item = new TagFrameItem(t_frame);
    m_tag_item->setExpanded(true);
    for (int i = 0; i < m_tag_item->childCount(); ++i) {
        TagFrameItem* temp = dynamic_cast<TagFrameItem*>(m_tag_item->child(i));
        if (frame < temp->get_frame()) {
            m_tag_item->insertChild(i, tf_item);
            blockSignals(true);
            setCurrentItem(tf_item);
            blockSignals(false);
            return;
        }
    }
    m_tag_item->addChild(tf_item);
    blockSignals(true);
    setCurrentItem(tf_item);
    blockSignals(false);
}

/**
 * @brief ProjectWidget::remove_frame_from_tag_item
 * Remove the frame from the current tag
 * @param frame
 */
void ProjectWidget::remove_frame_from_tag_item(int frame) {
    for (int i = 0; i < m_tag_item->childCount(); ++i) {
        TagFrameItem* tf_item = dynamic_cast<TagFrameItem*>(m_tag_item->child(i));
        if (tf_item->get_frame() == frame) {
            m_tag_item->removeChild(tf_item);
            return;
        }
    }
}

/**
 * @brief ProjectWidget::add_interval
 * Adds a new interval item under the video project
 * @param vid_proj
 * @param interval
 */
void ProjectWidget::add_interval(VideoProject* vid_proj, Interval* interval) {
    vid_proj->add_analysis(interval);
    VideoItem* vid_item = get_video_item(vid_proj);
    if (vid_item == nullptr) {
        set_status_bar("Something went wrong when adding interval: " + interval->get_name());
        return;
    }

    IntervalItem* item = new IntervalItem(interval);
    vid_item->addChild(item);
    clearSelection();
    item->setSelected(true);
    setCurrentItem(item);
    tree_item_changed(item);
    vid_item->setExpanded(true);
}


/**
 * @brief ProjectWidget::set_interval_area
 * Clear the current IntervalAreaItems and create new one for the current areas
 * This since some intervals might have merged together
 * @param start
 * @param end
 */
void ProjectWidget::set_interval_area(int new_area_start) {
    blockSignals(true);
    // Remove all current area items
    if (m_interval_item) {
        for (int i = m_interval_item->childCount()-1; i >= 0; --i) {
            m_interval_item->removeChild(m_interval_item->child(i));
        }
    }
    add_interval_area_to_interval(m_interval_item, new_area_start);
    m_interval_item->setExpanded(true);
    blockSignals(false);
}

/**
 * @brief ProjectWidget::add_interval_area_to_interval
 * Add all interval areas under the interval item
 * @param item
 * @param current
 */
void ProjectWidget::add_interval_area_to_interval(TreeItem* item, int current) {
    Interval* interval = dynamic_cast<IntervalItem*>(item)->get_interval();
    for (auto area : interval->m_area_list) {
        IntervalAreaItem* ia_item = new IntervalAreaItem(area.first, area.second);
        item->addChild(ia_item);
        if (ia_item->is_in_interval(current)) {
            setCurrentItem(item);
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
    VideoItem* vid_item = new VideoItem(vid_proj);

    if (vid_proj->get_video()->is_sequence()) {
        ImageSequence* sequence = dynamic_cast<ImageSequence*>(vid_proj->get_video());
        vid_item->setToolTip(0, sequence->get_search_path());

        // Tag all frames in the sequence if it's a tag sequence
        if (sequence->get_sequence_type() == TAG_SEQUENCE) {
            // Add the tag for the sequence
            Tag* tag = new Tag("Images", SEQUENCE_TAG);
            vid_proj->add_analysis(tag);
            vid_proj->tag_seq_tag = tag;

            // Add the sequence frame to the tag
            for (auto image : sequence->get_paths()) {
                QString hash = image.first;
                QString path = image.second;
                int frame = sequence->get_index_of_hash(hash);

                // Create the tagframe
                VideoState state;
                state.frame = frame;
                TagFrame* t_frame = new TagFrame(frame, state);
                t_frame->set_name(Utility::name_from_path(path));
                tag->add_frame(frame, t_frame);
            }
        }
    } else {
        vid_item->setToolTip(0, vid_proj->get_video()->file_path);
    }

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
    if (allowed_vid_exts.find(QString::fromStdString(ext)) != allowed_vid_exts.end()) {
        // Add file
        create_video(path);
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
        if (tmp.isDir() && tmp.absoluteFilePath().length() > path.length()) {
            folder_dropped(tmp.absoluteFilePath());
        }
        else {
            file_dropped(tmp.absoluteFilePath());
        }
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

void ProjectWidget::get_analysis_items(QTreeWidgetItem *root, std::vector<AnalysisItem *> &items) {
    for(int i = 0; i < root->childCount(); i++){
        QTreeWidgetItem* item = root->child(i);
        if(item->type() == ANALYSIS_ITEM) items.push_back(dynamic_cast<AnalysisItem*>(item));
        if(item->type() != ANALYSIS_ITEM) get_analysis_items(item, items);
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

        for (size_t i = 1; i < elems.size(); ++i) {
            if (item->child(std::stoi(elems[i]))) item = item->child(std::stoi(elems[i]));
        }

        // Create and add VideoItem
        if (item != nullptr && item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);

            v_item->set_video_project(vid_proj);
            auto vid = vid_proj->get_video();
            if (vid && vid->is_sequence()) {
                ImageSequence* sequence = dynamic_cast<ImageSequence*>(vid);
                v_item->setToolTip(0, sequence->get_search_path());
                v_item->load_sequence_items();
            } else {
                v_item->setToolTip(0, vid->file_path);
            }
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
        } else if (child->type() == TAG_ITEM) {
            TagItem* t_item = dynamic_cast<TagItem*>(child);
            t_item->rename();
        } else if (child->type() == ANALYSIS_ITEM ||
                   child->type() == DRAWING_TAG_ITEM ||
                   child->type() == INTERVAL_ITEM) {
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
        if (ana.second->get_type() == TAG || ana.second->get_type() == SEQUENCE_TAG) {
            TagItem* tag_item = new TagItem(dynamic_cast<Tag*>(ana.second));
            v_item->addChild(tag_item);
            add_frames_to_tag_item(tag_item);
        } else if (ana.second->get_type() == DRAWING_TAG) {
            DrawingTagItem* tag_item = new DrawingTagItem(dynamic_cast<Tag*>(ana.second));
            v_item->addChild(tag_item);
            add_frames_to_tag_item(tag_item);
        } else if (ana.second->get_type() == MOTION_DETECTION) {
            AnalysisItem* ana_item = new AnalysisItem(dynamic_cast<AnalysisProxy*>(ana.second));
            v_item->addChild(ana_item);
        } else if (ana.second->get_type() == INTERVAL) {
            IntervalItem* int_item = new IntervalItem(dynamic_cast<Interval*>(ana.second));
            v_item->addChild(int_item);
            add_interval_area_to_interval(int_item);
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

void ProjectWidget::update_analysis_settings() {
    std::vector<VideoItem*> v_items;
    AnalysisDialog* dialog = new AnalysisDialog(v_items, analysis_settings);
    dialog->show();
}

void ProjectWidget::advanced_analysis() {
    std::vector<VideoItem*> v_items;
    QTreeWidgetItem* s_item = invisibleRootItem();
    get_video_items(s_item, v_items);
    if(v_items.empty()) return;
    AnalysisSettings* new_settings = new AnalysisSettings(analysis_settings);
    AnalysisDialog* dialog = new AnalysisDialog(v_items, new_settings);
    connect(dialog, &AnalysisDialog::start_analysis, this, &ProjectWidget::start_analysis);
    dialog->show();
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
    delete_box.setMinimumSize(260,130);
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
        case QMessageBox::No:
                remove_list.clear();
                break;
    }
    return ok;
}

/**
 * @brief ProjectWidget::tree_item_changed
 * Slot function for when the current tree item is changed.
 * Performs different operations based on tree item type.
 * @param item          : new tree item
 * @param prev_item     : previous tree item
 */
void ProjectWidget::tree_item_changed(QTreeWidgetItem* item, QTreeWidgetItem* prev_item) {
    Q_UNUSED(prev_item)
    if (!item) return;
    switch(item->type()){
    case SEQUENCE_ITEM: {
        auto seq_item = dynamic_cast<SequenceItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent()->parent());
        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        state.frame = seq_item->get_index();
        emit set_video_project(vid_item->get_video_project());
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(false);
        emit set_interval_slider(false);
        emit enable_poi_btns(false,false);

        update_current_tag(vid_item);
        update_current_interval(vid_item);
        break;
    } case VIDEO_ITEM: {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        if (vid_item->get_video_project() == nullptr) return;
        emit set_video_project(vid_item->get_video_project());
        if (!vid_item->get_video_project()->get_video()->is_sequence()) {
            VideoState state;
            state = vid_item->get_video_project()->state;
            emit marked_video_state(vid_item->get_video_project(), state);
        }
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(false);
        emit set_interval_slider(false);
        emit enable_poi_btns(false,false);

        update_current_tag(vid_item);
        update_current_interval(vid_item);
        break;
    } case ANALYSIS_ITEM: {
        AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        if(!ana_item->is_finished()) break;
        ana_item->set_not_new();
        emit marked_analysis(ana_item->get_analysis());

        emit set_video_project(vid_item->get_video_project());
        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(true);
        emit set_detections(true);
        emit set_poi_slider(true);
        emit set_tag_slider(false);
        emit set_interval_slider(false);
        emit enable_poi_btns(true, true);

        update_current_tag(vid_item);
        update_current_interval(vid_item);

        AnalysisSettings* settings = dynamic_cast<BasicAnalysis*>(ana_item->get_analysis())->settings;
        emit update_settings_wgt(settings);
        break;
    } case DRAWING_TAG_ITEM: {
        DrawingTagItem* tag_item = dynamic_cast<DrawingTagItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        emit marked_basic_analysis(tag_item->get_tag());

        emit set_video_project(vid_item->get_video_project());
        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(true);
        emit set_interval_slider(false);
        emit enable_poi_btns(true, false);

        if (m_tag_item) m_tag_item->setCheckState(0, Qt::Unchecked);
        m_tag_item = nullptr;
        break;
    } case TAG_ITEM: {
        TagItem* tag_item = dynamic_cast<TagItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());

        emit marked_basic_analysis(tag_item->get_tag());

        if (!m_tag_item) {
            item->setCheckState(0, Qt::Checked);
            m_tag_item = tag_item;
        }
        // Deselect the old current tag and select the new one
        else if (m_tag_item != tag_item) {
            m_tag_item->setCheckState(0, Qt::Unchecked);
            item->setCheckState(0, Qt::Checked);
            m_tag_item = tag_item;
        }

        emit set_video_project(vid_item->get_video_project());
        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(true);
        emit set_interval_slider(false);
        emit enable_poi_btns(true, false);
        if (tag_item->get_tag()->get_type() == SEQUENCE_TAG) {
            emit seq_tag_btns(true);
        }
        break;
    } case TAG_FRAME_ITEM: {
        TagFrameItem* tf_item = dynamic_cast<TagFrameItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent()->parent());
        VideoProject* vp = vid_item->get_video_project();
        emit set_video_project(vp);

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(true);
        emit set_interval_slider(false);
        emit enable_poi_btns(true, false);

        VideoState state;
        state = tf_item->get_state();
        emit marked_video_state(vp, state);
        emit item_type(item->type());

        if (item->parent()->type() == TAG_ITEM) {
            TagItem* tag_item = dynamic_cast<TagItem*>(item->parent());
            emit marked_basic_analysis(tag_item->get_tag());
            if (tag_item->get_tag()->get_type() == SEQUENCE_TAG) {
                emit seq_tag_btns(true);
            }

            if (m_tag_item && m_tag_item != tag_item) m_tag_item->setCheckState(0, Qt::Unchecked);
            tag_item->setCheckState(0, Qt::Checked);
            m_tag_item = tag_item;
        } else if (item->parent()->type() == DRAWING_TAG_ITEM) {
            DrawingTagItem* dt_item = dynamic_cast<DrawingTagItem*>(item->parent());
            emit marked_basic_analysis(dt_item->get_tag());

            if (m_tag_item) m_tag_item->setCheckState(0, Qt::Unchecked);
            m_tag_item = nullptr;
        }
        break;
    } case INTERVAL_ITEM: {
        IntervalItem* interval_item = dynamic_cast<IntervalItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
        update_current_tag(vid_item);
        emit marked_basic_analysis(interval_item->get_interval());

        m_interval_item = interval_item;

        emit set_video_project(vid_item->get_video_project());
        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(false);
        emit set_interval_slider(true);
        emit enable_poi_btns(true, true);

        break;
    } case INTERVAL_AREA_ITEM: {
        IntervalAreaItem* ia_item = dynamic_cast<IntervalAreaItem*>(item);
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent()->parent());
        update_current_tag(vid_item);

        emit set_video_project(vid_item->get_video_project());

        emit set_zoom_tool();
        emit set_show_analysis_details(false);
        emit set_detections(false);
        emit set_poi_slider(false);
        emit set_tag_slider(false);
        emit set_interval_slider(true);
        emit enable_poi_btns(true, true);

        VideoState state;
        state = vid_item->get_video_project()->get_video()->state;
        state.frame = ia_item->get_start();
        emit marked_video_state(vid_item->get_video_project(), state);
        emit item_type(item->type());

        IntervalItem* interval_item = dynamic_cast<IntervalItem*>(item->parent());
        emit marked_basic_analysis(interval_item->get_interval());
        m_interval_item = interval_item;
        break;
    } case FOLDER_ITEM: {
        emit item_type(item->type());
        break;
    } default:
        break;
    }
    emit update_slider();
}

/**
 * @brief ProjectWidget::update_current_tag
 * Deselect the current tag if an item under a different video project is chosen
 * @param v_item
 */
void ProjectWidget::update_current_tag(VideoItem *v_item) {
    if (m_tag_item && dynamic_cast<VideoItem*>(m_tag_item->parent()) != v_item) {
        m_tag_item->setCheckState(0, Qt::Unchecked);
        emit marked_basic_analysis(nullptr);
        m_tag_item = nullptr;
    }
}

/**
 * @brief ProjectWidget::update_current_interval
 * Change/clear the current interval when a different one is chosed or a different video is chosen
 * @param v_item
 */
void ProjectWidget::update_current_interval(VideoItem* v_item) {
    if (m_interval_item && dynamic_cast<VideoItem*>(m_interval_item->parent()) != v_item) {
        emit marked_basic_analysis(nullptr);
        m_interval_item = nullptr;
    }
}

/**
 * @brief ProjectWidget::check_selection
 * Deselects all items that doesn't have selection_parent as parent
 */
void ProjectWidget::check_selection(){
    if (selectedItems().count() <= 1) return;
    blockSignals(true);
    for (auto i : selectedItems()) {
        if (i->parent() != selection_parent) i->setSelected(false);
    }
    blockSignals(false);
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

    QString item_text = item->text(0);
    switch(item->type()) {
    case INTERVAL_ITEM: {
        auto interval_item = dynamic_cast<IntervalItem*>(item);
        Interval* interval = interval_item->get_interval();
        if (interval->get_name() != item_text) interval->set_name(item_text);
        break;
    }
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
        menu.addAction("New Folder", this, &ProjectWidget::create_folder_item);
        menu.addAction("Import Video", this, &ProjectWidget::add_video);
        menu.addAction("Import Images", this, &ProjectWidget::add_images);
    } else if (item_count == 1) {
        // Clicked on item
        QTreeWidgetItem* item = selectedItems().front();
        switch (item->type()) {
        case INTERVAL_AREA_ITEM:
            menu.addAction("Add to video edit", this, [this, point] { add_to_video_edit(itemAt(point)); });
            menu.addAction("Delete", this, &ProjectWidget::remove_item);
            //menu.addAction("Add to video edit", this, &ProjectWidget::add_to_video_edit);
            break;
        case INTERVAL_ITEM:
        case TAG_ITEM:
            menu.addAction("Rename", this, &ProjectWidget::rename_item);
            menu.addAction("Delete", this, &ProjectWidget::remove_item);
            break;
        case DRAWING_TAG_ITEM:
            menu.addAction("Update", this, &ProjectWidget::drawing_tag);
            menu.addAction("Rename", this, &ProjectWidget::rename_item);
            menu.addAction("Delete", this, &ProjectWidget::remove_item);
            break;
        case ANALYSIS_ITEM:
            if (dynamic_cast<AnalysisItem*>(item)->is_finished()) {
                menu.addAction(show_details_act);
                menu.addAction(show_settings_act);
                menu.addAction("Rename", this, &ProjectWidget::rename_item);
                menu.addAction("Delete", this, &ProjectWidget::remove_item);
            }
            break;
        case FOLDER_ITEM:
            menu.addAction("Rename", this, &ProjectWidget::rename_item);
            menu.addAction("Delete", this, &ProjectWidget::remove_item);
            break;
        case VIDEO_ITEM:
            menu.addAction("Open video in widget", this, &ProjectWidget::open_video_in_widget);
            menu.addAction("Tag drawings", this, &ProjectWidget::drawing_tag);
            menu.addAction("Delete", this, &ProjectWidget::remove_item);
            break;
        case TAG_FRAME_ITEM: {
            if (item->parent()->type() == TAG_ITEM) {
                TagItem* t_item = dynamic_cast<TagItem*>(item->parent());
                Tag* tag = t_item->get_tag();
                if (tag->get_type() == TAG) {
                    menu.addAction("Update", this, &ProjectWidget::update_tag);
                }
                menu.addAction("Delete", this, &ProjectWidget::remove_item);
                break;
            } else if (item->parent()->type() == DRAWING_TAG_ITEM) {
                menu.addAction("Update", this, &ProjectWidget::update_tag);
            }
            break;
        }
        case SEQUENCE_ITEM:
            if (item->parent()->type() == SEQUENCE_CONTAINER_ITEM) {
                menu.addAction("Delete", this, &ProjectWidget::remove_item);
            }
            break;
        default:
            break;
        }
        menu.addSeparator();
        menu.addAction("New Folder", this, &ProjectWidget::create_folder_item);
        menu.addAction("Import Video", this, &ProjectWidget::add_video);
        menu.addAction("Import Images", this, &ProjectWidget::add_images);
    } else if (item_count > 1) {
        // Clicked whilst multiple items were selected
        menu.addAction("Delete", this, &ProjectWidget::remove_item);
    }
    menu.exec(mapToGlobal(point));
}

void ProjectWidget::open_video_in_widget() {
    VideoItem* v_item = dynamic_cast<VideoItem*>(currentItem());
    emit open_in_widget(v_item->get_video_project());
}

/**
 * @brief ProjectWidget::drawing_tag
 * Creates the drawing tag or updates the current one depending
 * on the current QTreeItem.
 * The drawing tag will tag all frames which have a drawing on them.
 */
void ProjectWidget::drawing_tag() {
    VideoItem* vid_item;
    // Remove the old drawing tag if update was selected
    if (selectedItems().front()->type() == DRAWING_TAG_ITEM) {
        vid_item = dynamic_cast<VideoItem*>(selectedItems().front()->parent());
        // Update tag drawing
        DrawingTagItem* item = dynamic_cast<DrawingTagItem*>(selectedItems().front());
        vid_item->get_video_project()->remove_analysis(item->get_tag());
        delete selectedItems().front();
    } else if (selectedItems().front()->type() == VIDEO_ITEM) {
        vid_item = dynamic_cast<VideoItem*>(selectedItems().front());
    } else {
        return;
    }
    // Create tag drawing
    Tag* tag = new Tag("Drawing tag", DRAWING_TAG);

    // Add all drawings to tag frame items.
    VideoProject* vid_proj = vid_item->get_video_project();
    for (auto const& frame_overlay : vid_proj->get_overlay()->get_overlays()) {
        if (frame_overlay.second.size() > 0) {
            VideoState state;
            state.frame = frame_overlay.first;
            TagFrame* t_frame = new TagFrame(frame_overlay.first, state);
            tag->add_frame(frame_overlay.first, t_frame);
        }
    }
    // Add tag to tree
    add_tag(vid_proj, tag);
}

void ProjectWidget::add_to_video_edit(QTreeWidgetItem* item) {
    int start, end;
    VideoProject* vid_proj = nullptr;
    if (item->type() == INTERVAL_AREA_ITEM) {
        IntervalAreaItem* ia_item = dynamic_cast<IntervalAreaItem*>(item);
        VideoItem* v_item = dynamic_cast<VideoItem*>(item->parent()->parent());
        start = ia_item->get_start();
        end = ia_item->get_end();
        vid_proj = v_item->get_video_project();
    } else {
        return;
    }
    emit interval_to_edit(start, end, vid_proj);

}

/**
 * @brief ProjectWidget::toggle_details
 * Slot function for settings the action's checkbox
 * @param b
 */
void ProjectWidget::toggle_details(bool b) {
    show_details_act->setChecked(b);
}

/**
 * @brief ProjectWidget::toggle_settings
 * Slot function for settings the action's checkbox
 * @param b
 */
void ProjectWidget::toggle_settings(bool b) {
    show_settings_act->setChecked(b);
}

/**
 * @brief ProjectWidget::update_settings
 * Updates the analysis settings dock window with the settings
 * from the clicked analysis
 */
void ProjectWidget::update_settings() {
    QTreeWidgetItem* item = selectedItems().front();
    if (item->type() == ANALYSIS_ITEM) {
        AnalysisItem* a_item = dynamic_cast<AnalysisItem*>(item);
        if (!a_item->is_finished()) return;
        AnalysisSettings* settings = dynamic_cast<BasicAnalysis*>(a_item->get_analysis())->settings;
        emit update_settings_wgt(settings);
        emit show_analysis_settings(true);
    }
}

/**
 * @brief ProjectWidget::remove_item
 * Deletes all selected items.
 * If a folder is selected then it will delete all subitems as well.
 */
void ProjectWidget::remove_item() {
    if (!currentItem()) return;
    if (currentItem()->type() == ANALYSIS_ITEM && !dynamic_cast<AnalysisItem*>(currentItem())->is_finished()) {
        return;
    } else if (currentItem()->type() == VIDEO_ITEM) {
        if (is_analysis_running(currentItem())) return;
    }

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
    switch (item->type()) {
    case FOLDER_ITEM:
        while (item->childCount() != 0) {
            remove_tree_item(item->child(0));
        }
        break;
    case VIDEO_ITEM:
        remove_video_item(item);
        break;
    case TAG_ITEM:
        remove_tag_item(item);
        break;
    case DRAWING_TAG_ITEM:
        remove_drawing_tag_item(item);
        break;
    case ANALYSIS_ITEM:
        remove_analysis_item(item);
        break;
    case TAG_FRAME_ITEM:
        remove_tag_frame_item(item);
        break;
    case SEQUENCE_ITEM:
        remove_sequence_item(item);
        break;
    case INTERVAL_ITEM:
        remove_interval_item(item);
        break;
    case INTERVAL_AREA_ITEM:
        remove_interval_area_item(item);
        break;
    default:
        break;
    }
    blockSignals(true);
    delete item;
    blockSignals(false);
    setCurrentItem(nullptr);
}

/**
 * @brief ProjectWidget::remove_video_item
 * Remove and clean video item
 * @param item
 */
void ProjectWidget::remove_video_item(QTreeWidgetItem *item) {
    VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
    VideoProject* v_proj = vid_item->get_video_project();

    // Remove all children
    while (item->childCount() != 0) {
        remove_tree_item(item->child(0));
    }
    // Remove the video from the list of videos
    if (v_proj->get_video()->is_sequence()) {
        removed_sequences.push_back(v_proj);
    } else {
        auto video_it = std::find(video_list.begin(), video_list.end(), v_proj->get_video());
        video_list.erase(video_it);
        m_proj->remove_video_project(v_proj);
    }
    emit item_removed(v_proj);
    emit remove_overlay();
}

/**
 * @brief ProjectWidget::remove_tag_item
 * Remove and clean tag item
 * @param item
 */
void ProjectWidget::remove_tag_item(QTreeWidgetItem *item) {
    emit set_tag_slider(false);
    VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
    Tag* tag = dynamic_cast<TagItem*>(item)->get_tag();
    if (tag->get_type() != SEQUENCE_TAG) {
        vid_item->get_video_project()->remove_analysis(tag);
    }
    m_tag_item = nullptr;
    emit marked_basic_analysis(nullptr);
}

/**
 * @brief ProjectWidget::remove_drawing_item
 * Remove and clean drawing tag item
 * @param item
 */
void ProjectWidget::remove_drawing_tag_item(QTreeWidgetItem* item) {
    emit set_tag_slider(false);

    VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
    Tag* tag = dynamic_cast<DrawingTagItem*>(item)->get_tag();
    vid_item->get_video_project()->remove_analysis(tag);
}

/**
 * @brief ProjectWidget::remove_analysis_item
 * Remove and clean analysis item
 * @param item
 */
void ProjectWidget::remove_analysis_item(QTreeWidgetItem* item) {
    emit set_detections(false);
    emit set_poi_slider(false);
    emit set_interval_slider(false);
    emit enable_poi_btns(false, false);

    VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
    AnalysisProxy* analysis = dynamic_cast<AnalysisItem*>(item)->get_analysis();

    remove_list.push_back(analysis->full_path());
    vid_item->get_video_project()->remove_analysis(analysis);
    emit clear_analysis();
}

/**
 * @brief ProjectWidget::remove_tag_frame_item
 * Remove and clean tagframe item
 * @param item
 */
void ProjectWidget::remove_tag_frame_item(QTreeWidgetItem *item) {
    TagItem* tag_item = dynamic_cast<TagItem*>(item->parent());
    TagFrameItem* tf_item = dynamic_cast<TagFrameItem*>(item);
    Tag* tag = tag_item->get_tag();
    int frame = tf_item->get_frame();
    //tag->remove_frame(frame);
    if (tag->get_type() == SEQUENCE_TAG) {
        // TODO remove the tagframe from the sequence and from folder
        // something like this
        VideoItem* vid_item = dynamic_cast<VideoItem*>(tag_item->parent());
        if (vid_item) {
            ImageSequence* sequence = dynamic_cast<ImageSequence*>(vid_item->get_video_project()->get_video());
            if (sequence) {
                bool res = sequence->remove_image_with_index(frame);
                if (!res) return;
            }
            // Remove frame and update the index of all other tags
            tag->remove_frame(frame);
            tag->update_index_tag();
            emit new_slider_max(-1);

            // Update drawing widget
            emit remove_frame(frame);

            tree_item_changed(item->parent()->parent());
        }
    } else {
        tag->remove_frame(frame);   // untag
    }
}

void ProjectWidget::remove_sequence_item(QTreeWidgetItem *item) {
    SequenceItem* seq_item = dynamic_cast<SequenceItem*>(item);
    if (seq_item) {
        emit remove_frame(seq_item->get_index());
        seq_item->remove();
    }
}

/**
 * @brief ProjectWidget::remove_interval_item
 * Remove and clean interval item
 * @param item
 */
void ProjectWidget::remove_interval_item(QTreeWidgetItem* item) {
    emit set_interval_slider(false);

    VideoItem* vid_item = dynamic_cast<VideoItem*>(item->parent());
    Interval* interval = dynamic_cast<IntervalItem*>(item)->get_interval();
    vid_item->get_video_project()->remove_analysis(interval);
    m_interval_item = nullptr;
    emit marked_basic_analysis(nullptr);
}

/**
 * @brief ProjectWidget::remove_interval_area_item
 * Remove and clean interval area item
 * @param item
 */
void ProjectWidget::remove_interval_area_item(QTreeWidgetItem* item) {
    IntervalItem* interval_item = dynamic_cast<IntervalItem*>(item->parent());
    IntervalAreaItem* ia_item = dynamic_cast<IntervalAreaItem*>(item);
    int frame = ia_item->get_start();
    interval_item->get_interval()->remove_area_by_frame(frame);
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
    } else {        // Tag frame item
        insertTopLevelItem(topLevelItemCount(), item);
    }
    editItem(item);
    clearSelection();
    item->setSelected(true);
    m_proj->set_unsaved(true);
}

/**
 * @brief ProjectWidget::is_analysis_running
 * Searches for all analysisitems under root
 * to see if anyone of them is currently running.
 * @param root
 * @return
 */
bool ProjectWidget::is_analysis_running(QTreeWidgetItem* root) {
    if (root == nullptr) { root = invisibleRootItem(); }

    std::vector<AnalysisItem*> a_items;
    get_analysis_items(root, a_items);
    bool analysis_running = false;
    for (AnalysisItem* item : a_items) {
        if (!item->is_finished()) {
            analysis_running = true;
            break;
        }
    }

    if (analysis_running) {
        QString text = "One or more analyses are currently running";
        QString info_text = "Let them finish or stop them before continuing";
        QMessageBox msg_box;
        msg_box.setMinimumSize(300,120);
        msg_box.setText(text);
        msg_box.setInformativeText(info_text);
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
    return analysis_running;
}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
bool ProjectWidget::save_project() {
    if (m_proj == nullptr || is_analysis_running()) return false;

    for (QString path : remove_list) {
        QFile file(path);
        if(file.exists()) {
            file.remove();
        }
    }
    remove_list.clear();

    // Move all project files if the current project is temporary
    // i.e. has not been saved yet
    if (m_proj->is_temporary()) {
        QString name{}, path{};
        ProjectDialog* project_dialog = new ProjectDialog(&name, &path, this, Constants::DEFAULT_PATH);
        connect(project_dialog, &ProjectDialog::open_project, this, &ProjectWidget::open_project);
        int status = project_dialog->exec();

        if (status == project_dialog->Accepted) {
            // User clicked ok, dialog checked for proper path & name
            // Update project path
            m_proj->copy_directory_files(m_proj->get_dir(), path + name, true, std::vector<QString>{"vian"});
            m_proj->remove_files();
            m_proj->set_name_and_path(name, path);
            m_proj->set_temporary(false);
            set_main_window_name(name);
            emit set_project(m_proj);
            QDir dir;
            dir.mkpath(m_proj->get_dir());

            // If the current video is a sequence then it needs to be reloaded
            // since the images path will have changed
            for (auto vid_proj : m_proj->get_videos()) {
                if (vid_proj->is_current() && vid_proj->get_video()->is_sequence()) {
                    vid_proj->set_current(false);
                    emit marked_video_state(vid_proj, vid_proj->get_video()->state);
                }
            }
        } else {
            // User aborted dialog, cancel save
            return false;
        }
    }

    for (auto v_proj : removed_sequences) {
        auto seq = dynamic_cast<ImageSequence*>(v_proj->get_video());
        QDir directory(seq->get_search_path());
        directory.removeRecursively();
        delete v_proj;
    }
    removed_sequences.clear();

    save_item_data();
    emit save_draw_wgt();
    emit save_bmark_wgt();
    emit save_videdit_wgt();

    // Mark all analysis as saved
    std::vector<AnalysisItem*> a_items;
    get_analysis_items(invisibleRootItem(), a_items);
    for (AnalysisItem* item : a_items) {
        item->saved = true;
    }

    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.save_state(m_proj->get_dir() + "treestate");

    m_proj->save_project();

    RecentProject rp;
    rp.update_recent(m_proj->get_name(), m_proj->get_file(), m_proj->get_last_changed());
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
    auto new_proj = Project::fromFile(project_path);
    if (new_proj == nullptr) {
        qWarning() << "Something went wrong while creating the temporary folder.";
        return false;
    }
    m_proj = new_proj;
    set_status_bar("Opening project");

    // Load project tree structure
    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.load_state(m_proj->get_dir() + "treestate");
    set_main_window_name(m_proj->get_name());
    emit set_project(m_proj);
    for (auto vid_proj : m_proj->get_videos()) {
        insert_to_path_index(vid_proj);

        if (vid_proj->get_video()->is_sequence()) continue;
        video_list.push_back(vid_proj->get_video());
    }
    ViewPathDialog* path_dialog = new ViewPathDialog(video_list);
    // If not all paths in the project are valid, open the Viewpathdialog
    if (!path_dialog->all_valid()) {
        int status = path_dialog->exec();

        if (status == path_dialog->Accepted) {
            update_videoitems();
        }
    }
    return true;
}

/**
 * @brief ProjectWidget::close_project
 * Closes the current project if there is one and user accepts prompt
 * Returns true if the project has been closed.
 */
bool ProjectWidget::close_project() {
    if (m_proj == nullptr) return true;

    std::vector<AnalysisItem*> a_items;
    get_analysis_items(invisibleRootItem(), a_items);
    bool analysis_running = false;
    for (AnalysisItem* item : a_items) {
        if (!item->is_finished()) {
            analysis_running = true;
        }
    }

    if (analysis_running) {
        QString text = "One or more analyses are currently running";
        QString info_text = "Are you sure you wanna continue? Doing so will stop the analyses.";
        if (message_box(text, info_text, true)) {
            emit abort_all_analysis();
        } else {
            return false;
        }
    }

    // Prompt user to save. If cancel keep the current project
    bool abort_close = false;
    if (!m_proj->is_saved()){
        abort_close = !prompt_save();
        if (abort_close) {
            return false;
        }
    }

    // Remove the analysis file from the folder of all the analyses that are not saved
    for (AnalysisItem* item : a_items) {
        if (!(item->saved)) {
            AnalysisProxy* analysis = item->get_analysis();
            analysis->delete_saveable(analysis->full_path());
        }
    }

    emit close_all_widgets();

    emit set_show_analysis_details(false);
    emit set_detections(false);
    emit set_poi_slider(false);
    emit set_tag_slider(false);
    emit set_interval_slider(false);
    emit enable_poi_btns(false, false);

    // If the user chose not to save
    // Restore changes on all image sequences that are unsaved but not removed
    if (!m_proj->is_saved()) {
        for (auto vid_proj : m_proj->get_videos()) {
            // Remove folder if sequence has been removed (never added) from the project
            if (vid_proj->get_video()->is_sequence()) {
                auto sequence = dynamic_cast<ImageSequence*>(vid_proj->get_video());
                if (sequence->never_saved()) {
                    // Remove sequences that have never been saved
                    QDir directory(sequence->get_search_path());
                    directory.removeRecursively();
                } else if (!sequence->is_saved()) {
                    // Revert changes done to sequences
                    sequence->revert();
                    // Revert the changes done to the tag_map
                    if (vid_proj->get_video()->get_sequence_type() == TAG_SEQUENCE) {
                        vid_proj->tag_seq_tag->revert_tag_map();
                    }
                }
            }
        }
    }

    // If the user chose to save, remove all folders of the removed image sequences
    // Otherwise, revert the changes made
    if (!m_proj->is_saved()) {
        for (auto v_proj : removed_sequences) {
            auto seq = dynamic_cast<ImageSequence*>(v_proj->get_video());
            seq->revert();
            delete v_proj;
        }
    }

    // Remove project if temporary
    if (m_proj->is_temporary()) {
        m_proj->remove_files();
    }

    emit set_status_bar("Closing project");
    emit project_closed();
    emit remove_overlay();

    video_list.clear();
    delete m_proj;
    m_proj = nullptr;
    m_tag_item = nullptr;
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
    clear();
    delete m_proj;
    m_proj = nullptr;
    emit project_closed();
    emit remove_overlay();
    new_project();
}

void ProjectWidget::set_main_window_name(QString name) {
    parentWidget()->parentWidget()->setWindowTitle("ViAn  -  " + name);
}

void ProjectWidget::update_current_videoitem(QString path) {
    if (currentItem()->type() != VIDEO_ITEM) return;
    QString name = Utility::name_from_path(path);
    currentItem()->setText(0, name);
    currentItem()->setToolTip(0, path);
    VideoItem* v_item = dynamic_cast<VideoItem*>(currentItem());
    v_item->get_video_project()->get_video()->set_name(name);
    v_item->set_thumbnail();
}

void ProjectWidget::update_videoitems() {
    for (auto item : findItems(".", Qt::MatchContains, 0)) {
        if (item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            Video* video = v_item->get_video_project()->get_video();
            v_item->setText(0, video->get_name());
            v_item->setToolTip(0, video->file_path);
            v_item->set_thumbnail();
        }
    }
}

/**
 * @brief ProjectWidget::select_video_project
 * Set vid_proj to current project with the state state.
 * If vid_proj already is current just set state else
 * iterate over the vid_items to find the correct one in the tree
 *
 * This is used from the bookmarks to select the video in the projecttree
 * when clicking through bookmarks
 * @param vid_proj
 * @param state
 */
void ProjectWidget::select_video_project(VideoProject* vid_proj, VideoState state) {
    if (vid_proj->is_current()) {
        vid_proj->state = state;
        emit marked_video_state(vid_proj, state);
    } else {
        std::vector<VideoItem*> v_items;
        QTreeWidgetItem* s_item = invisibleRootItem();
        get_video_items(s_item, v_items);
        for (auto vid_item : v_items) {
            if (vid_item->get_video_project() == vid_proj) {
                vid_item->get_video_project()->state = state;
                setCurrentItem(vid_item);
                return;
            }
        }
    }
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
    msg_box.setMinimumSize(310,125);
    msg_box.setText(text);
    msg_box.setInformativeText(info_text);
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();
    return reply == QMessageBox::Yes;
}
