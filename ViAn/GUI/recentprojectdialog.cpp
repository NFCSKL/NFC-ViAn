#include "recentprojectdialog.h"

RecentProjectDialog::RecentProjectDialog(QWidget* parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::WindowStaysOnTopHint);
    h_layout = new QHBoxLayout();
    v_main_layout = new QVBoxLayout(this);
    v_btn_layout = new QVBoxLayout();
    v_btn_layout->setAlignment(Qt::AlignTop);


    v_main_layout->addWidget(new QLabel(tr("Recent projects:"))); // First row
    v_main_layout->addLayout(h_layout);

    recent_list = new QListWidget();
    h_layout->addWidget(recent_list);                           // Second row first col
    h_layout->addLayout(v_btn_layout);                          // Second row second col


    new_btn = new QPushButton(tr("New project"));
    new_btn->setToolTip(tr("Start a new project"));
    browse_btn = new QPushButton(tr("Browse..."));
    browse_btn->setToolTip(tr("Browse a project from file"));
    open_btn = new QPushButton(tr("Open"));
    open_btn->setToolTip(tr("Open selected project"));
    v_btn_layout->addWidget(new_btn);                           // Second row second col first row
    v_btn_layout->addWidget(browse_btn);                          // Second row second col second row
    v_btn_layout->addWidget(open_btn);                          // Second row second col third row

    for (auto project : RecentProject().load_recent()) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(project.first));
        item->setToolTip(QString::fromStdString(project.second));
        recent_list->addItem(item);
    }

    connect(recent_list, &QListWidget::itemDoubleClicked, this, &RecentProjectDialog::on_item_double_clicked);
    connect(new_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_new_btn_clicked);
    connect(browse_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_browse_btn_clicked);
    connect(open_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_open_btn_clicked);
}

/**
 * @brief RecentProjectDialog::on_item_double_clicked
 * Accepts dialog and emits signal to open the clicked project
 * @param item
 */
void RecentProjectDialog::on_item_double_clicked(QListWidgetItem* item) {
    accept();
    emit open_project(item->toolTip());
}

/**
 * @brief RecentProjectDialog::on_new_btn_clicked
 * Accepts dialog and emits signal to create a new project
 */
void RecentProjectDialog::on_new_btn_clicked(){
    new_project();
    accept();
}

/**
 * @brief RecentProjectDialog::on_open_btn_clicked
 * Accepts dialog and emits signal to open a project from file
 */
void RecentProjectDialog::on_browse_btn_clicked(){
    accept();
    open_project_from_file();
}

void RecentProjectDialog::on_open_btn_clicked() {
    if (recent_list->selectedItems().length() == 0) return;
    open_project(recent_list->currentItem()->toolTip());
    accept();
}
