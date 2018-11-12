#include "viewpathdialog.h"

#include "Project/video.h"
#include "viewpathitem.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QUrl>

ViewPathDialog::ViewPathDialog(std::vector<Video*> video_list, QWidget* parent) : QDialog(parent) {
    setWindowTitle("Vian - View paths");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(500,200);

    m_video_list = video_list;

    QVBoxLayout* v_layout = new QVBoxLayout(this);
    QHBoxLayout* h_layout = new QHBoxLayout();

    path_list = new QTreeWidget(this);
    path_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    path_list->setColumnCount(NUM_COLUMNS);
    path_list->header()->resizeSection(0,40);

    path_list->headerItem()->setText(0, "Status");
    path_list->headerItem()->setText(1, "Video name");
    path_list->headerItem()->setText(2, "Path");
    add_paths(path_list);

    QVBoxLayout* btn_layout = new QVBoxLayout();
    btn_layout->setAlignment(Qt::AlignTop);

    QPushButton* open_folder_btn = new QPushButton(tr("Open folder"));
    QPushButton* update_path_btn = new QPushButton(tr("Update path"));
    QPushButton* update_folder_btn = new QPushButton(tr("Update folder"));

    connect(open_folder_btn, &QPushButton::clicked, this, &ViewPathDialog::open_folder_btn_clicked);
    connect(update_path_btn, &QPushButton::clicked, this, &ViewPathDialog::update_path_btn_clicked);
    connect(update_folder_btn, &QPushButton::clicked, this, &ViewPathDialog::update_folder_btn_clicked);

    btn_layout->addWidget(open_folder_btn);
    btn_layout->addWidget(update_path_btn);
    btn_layout->addWidget(update_folder_btn);

    h_layout->addWidget(path_list);
    h_layout->addLayout(btn_layout);

    // Add Buttons
    btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ViewPathDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ViewPathDialog::cancel_btn_clicked);

    v_layout->addLayout(h_layout);
    v_layout->addWidget(btn_box);
    setLayout(v_layout);
}

/**
 * @brief ViewPathDialog::add_paths
 * Add all videos in the video list to the treewidget
 * This will set the :
 * first column with a icon indicating if video can be read
 * second column with the video name
 * third column with the video path
 * @param tree
 */
void ViewPathDialog::add_paths(QTreeWidget* tree) {
    for (Video* video : m_video_list) {
        ViewPathItem* item = new ViewPathItem(video, tree);
        set_icon(item);
    }
}

/**
 * @brief ViewPathDialog::set_icon
 * Checks the item's path to see if there is a video file there
 * Then updates the item's icon to indicate success or failure.
 * @param item
 */
void ViewPathDialog::set_icon(ViewPathItem* item) {
    QFile load_file(item->get_path());
    if (load_file.open(QIODevice::ReadOnly)) {     // Attempt to open file
        item->setIcon(0, check_icon);
        item->set_valid(true);
    } else {
        item->setIcon(0, cross_icon);
        item->set_valid(false);
    }
}

/**
 * @brief ViewPathDialog::all_valid
 * Returns if all the videopaths are valid
 * That means the video file in the path can be read.
 * @return
 */
bool ViewPathDialog::all_valid() {
    for (auto i = 0; i < path_list->topLevelItemCount(); ++i) {
        ViewPathItem* vp_item = dynamic_cast<ViewPathItem*>(path_list->topLevelItem(i));
        if (!vp_item->is_valid()) return false;
    }
    return true;
}

/**
 * @brief ViewPathDialog::open_folder_btn_clicked
 * Slot function for open_folder button
 * Open the folder where the selected items is.
 */
void ViewPathDialog::open_folder_btn_clicked() {
    if (path_list->selectedItems().length() != 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDesktopServices::openUrl(QUrl("file:///"+v_item->get_dir(), QUrl::TolerantMode));
}

/**
 * @brief ViewPathDialog::update_path_btn_clicked
 * Slot function for update_path button
 * Open a dialog where the user can select a new path
 * where the video is.
 */
void ViewPathDialog::update_path_btn_clicked() {
    if (path_list->selectedItems().length() != 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDir standard;
    standard.mkpath(v_item->get_dir());
    QString file_path = QFileDialog::getOpenFileName(this,
                                                tr("Choose project path"),
                                                v_item->get_dir());
    // Update the path for the selected item.
    if(!file_path.isEmpty()) {
        v_item->set_path(file_path);
        set_icon(v_item);
    }
}

/**
 * @brief ViewPathDialog::update_folder_btn_clicked
 * Slot function for update_folder button
 * Open a dialog where the user can select a new folder
 * where the videos are.
 */
void ViewPathDialog::update_folder_btn_clicked() {
    // Can be done on multiple items
    if (path_list->selectedItems().length() < 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDir standard;
    standard.mkpath(v_item->get_dir());
    QString new_dir = QFileDialog::getExistingDirectory(this, tr("Choose new directory"),
                                                         v_item->get_dir());
    // Update the directory for all selected items
    for (auto item : path_list->selectedItems()) {
        auto v_item = dynamic_cast<ViewPathItem*>(item);
        if(!new_dir.isEmpty()) {
            v_item->set_dir(new_dir);
            set_icon(v_item);
        }
    }
}

/**
 * @brief ViewPathDialog::ok_btn_clicked
 * Slot function for ok button
 */
void ViewPathDialog::ok_btn_clicked() {
    for (auto item : path_list->findItems("", Qt::MatchContains, 1)) {
        auto v_item = dynamic_cast<ViewPathItem*>(item);
        if (v_item->is_item_changed()) {
            v_item->update_video();
        }
    }
    accept();
}

/**
 * @brief ViewPathDialog::cancel_btn_clicked
 * Slot function for cancel button
 */
void ViewPathDialog::cancel_btn_clicked() {
    reject();
}
