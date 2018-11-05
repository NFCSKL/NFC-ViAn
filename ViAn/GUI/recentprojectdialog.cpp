#include "recentprojectdialog.h"

#include "Project/recentproject.h"

#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>


RecentProjectDialog::RecentProjectDialog(QWidget* parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint | Qt::WindowStaysOnTopHint));
    setWindowTitle("ViAn - Recent projects");
    setWindowIcon(QIcon("../ViAn/Icons/home.png"));
    setMinimumSize(500,300);
    h_layout = new QHBoxLayout();
    v_main_layout = new QVBoxLayout(this);
    v_btn_layout = new QVBoxLayout();
    v_btn_layout->setAlignment(Qt::AlignTop);

    v_main_layout->addWidget(new QLabel(tr("Recent projects:"))); // First row
    v_main_layout->addLayout(h_layout);

    recent_list = new QTreeWidget();
    recent_list->setColumnCount(NUM_COLUMNS);
    recent_list->headerItem()->setText(0, "Project");
    recent_list->headerItem()->setText(1, "Last changed");
    recent_list->setRootIsDecorated(false);                     // Remove the empty space to the left of the item

    h_layout->addWidget(recent_list);                           // Second row first col
    h_layout->addLayout(v_btn_layout);                          // Second row second col

    new_btn = new QPushButton(tr("New project"));
    new_btn->setToolTip(tr("Start a new project"));
    browse_btn = new QPushButton(tr("Browse..."));
    browse_btn->setToolTip(tr("Browse a project from file"));
    open_btn = new QPushButton(tr("Open"));
    open_btn->setToolTip(tr("Open selected project"));
    remove_btn = new QPushButton(tr("Remove"));
    remove_btn->setToolTip("Remove selected project");
    v_btn_layout->addWidget(new_btn);                           // Second row second col first row
    v_btn_layout->addWidget(browse_btn);                          // Second row second col second row
    v_btn_layout->addWidget(open_btn);                          // Second row second col third row
    v_btn_layout->addWidget(remove_btn);

    for (auto project : RecentProject().load_recent()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(recent_list);
        item->setText(0, QString::fromStdString(std::get<0>(project)));     // Set name
        item->setText(1, QString::fromStdString(std::get<2>(project)));     // Set the last changed date
        item->setToolTip(0, QString::fromStdString(std::get<1>(project)));
        recent_list->addTopLevelItem(item);
    }

    connect(recent_list, &QTreeWidget::itemSelectionChanged, this, &RecentProjectDialog::item_selection_changed);
    connect(recent_list, &QTreeWidget::itemDoubleClicked, this, &RecentProjectDialog::on_item_double_clicked);
    connect(new_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_new_btn_clicked);
    connect(browse_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_browse_btn_clicked);
    connect(open_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_open_btn_clicked);
    connect(remove_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_remove_btn_clicked);
}

RecentProjectDialog::~RecentProjectDialog() {
    recent_list->clear();
}

/**
 * @brief RecentProjectDialog::on_item_clicked
 * Selects the open button
 * @param item
 */
void RecentProjectDialog::item_selection_changed() {
    if (recent_list->selectedItems().length() > 0) {
        open_btn->setDefault(true);
    } else {
        new_btn->setDefault(true);
    }
}

/**
 * @brief RecentProjectDialog::on_item_double_clicked
 * Accepts dialog and emits signal to open the clicked project
 * @param item
 */
void RecentProjectDialog::on_item_double_clicked(QTreeWidgetItem* item) {
    accept();
    emit open_project(item->toolTip(0));
}

/**
 * @brief RecentProjectDialog::on_new_btn_clicked
 * Accepts dialog and emits signal to create a new project
 */
void RecentProjectDialog::on_new_btn_clicked(){
    emit new_project();
    accept();
}

/**
 * @brief RecentProjectDialog::on_open_btn_clicked
 * Accepts dialog and emits signal to open a project from file
 */
void RecentProjectDialog::on_browse_btn_clicked(){
    QString project_path = QFileDialog().getOpenFileName(
                this,
                tr("Open project"),
                QDir::homePath(),
                "*.vian");

    if (project_path.isEmpty()) return;

    accept();
    emit open_project_from_file(project_path);
}

void RecentProjectDialog::on_open_btn_clicked() {
    if (recent_list->selectedItems().length() == 0) return;
    emit open_project(recent_list->currentItem()->toolTip(0));
    accept();
}

void RecentProjectDialog::on_remove_btn_clicked() {
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Warning);
    msg_box.setMinimumSize(370, 110);
    msg_box.setText("Are you sure you want to remove the selected projects?");
    msg_box.setInformativeText("This will delete all project files (images, reports, etc).");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();
    if (reply == QMessageBox::Yes) {
        QString file = recent_list->currentItem()->toolTip(0);
        QString substr = file.left(file.lastIndexOf('/') + 1);
        QDir path(substr);
        if (path.exists()) {
            path.removeRecursively();
        }
        delete recent_list->currentItem();
    }
}
