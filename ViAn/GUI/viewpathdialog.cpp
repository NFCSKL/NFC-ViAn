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

    QPushButton* folder_btn = new QPushButton(tr("Open folder"));
    QPushButton* browse_btn = new QPushButton(tr("Update path"));
    QPushButton* path_btn = new QPushButton(tr("Update folder"));

    connect(folder_btn, &QPushButton::clicked, this, &ViewPathDialog::folder_btn_clicked);
    connect(browse_btn, &QPushButton::clicked, this, &ViewPathDialog::browse_btn_clicked);
    connect(path_btn, &QPushButton::clicked, this, &ViewPathDialog::path_btn_clicked);

    btn_layout->addWidget(folder_btn);
    btn_layout->addWidget(browse_btn);
    btn_layout->addWidget(path_btn);

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

void ViewPathDialog::add_paths(QTreeWidget* tree) {
    for (Video* video : m_video_list) {
        ViewPathItem* item = new ViewPathItem(video, tree);
        set_icon(item);
    }
}

void ViewPathDialog::set_icon(ViewPathItem* item) {
    QFile load_file(item->get_path());
    if (load_file.open(QIODevice::ReadOnly)) {     // Attempt to open file
        item->setIcon(0, check_icon);
    } else {
        item->setIcon(0, cross_icon);
    }
}

void ViewPathDialog::folder_btn_clicked() {
    if (path_list->selectedItems().length() != 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDesktopServices::openUrl(QUrl("file:///"+v_item->get_dir(), QUrl::TolerantMode));
}

void ViewPathDialog::browse_btn_clicked() {
    if (path_list->selectedItems().length() != 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDir standard;
    standard.mkpath(v_item->get_dir());
    QString file_path = QFileDialog::getOpenFileName(this,
                                                tr("Choose project path"),
                                                v_item->get_dir());
    if(!file_path.isEmpty()) {
        v_item->set_path(file_path);
        set_icon(v_item);
    }
}

void ViewPathDialog::path_btn_clicked() {
    if (path_list->selectedItems().length() < 1) return;
    auto v_item = dynamic_cast<ViewPathItem*>(path_list->currentItem());
    QDir standard;
    standard.mkpath(v_item->get_dir());
    QString new_dir = QFileDialog::getExistingDirectory(this, tr("Choose new directory"),
                                                         v_item->get_dir());
    for (auto item : path_list->selectedItems()) {
        auto v_item = dynamic_cast<ViewPathItem*>(item);
        if(!new_dir.isEmpty()) {
            v_item->set_dir(new_dir);
            set_icon(v_item);
        }
    }
}

void ViewPathDialog::ok_btn_clicked() {
    for (auto item : path_list->findItems("", Qt::MatchContains, 1)) {
        auto v_item = dynamic_cast<ViewPathItem*>(item);
        if (v_item->is_item_changed()) {
            v_item->update_video();
        }


        // Update the video items with the new video name and tooltips
        // like in proj wgt (1426)
    }
    accept();
}

void ViewPathDialog::cancel_btn_clicked() {
    reject();
}
