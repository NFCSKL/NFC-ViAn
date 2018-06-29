#include <QFileDialog>
#include <QHeaderView>
#include "recentprojectdialog.h"
#include <QDebug>

RecentProjectDialog::RecentProjectDialog(QWidget* parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint | Qt::WindowStaysOnTopHint));
    this->setWindowTitle("ViAn - Recent projects");
    h_layout = new QHBoxLayout();
    v_main_layout = new QVBoxLayout(this);
    v_btn_layout = new QVBoxLayout();
    v_btn_layout->setAlignment(Qt::AlignTop);


    v_main_layout->addWidget(new QLabel(tr("Recent projects:"))); // First row
    v_main_layout->addLayout(h_layout);

    recent_list = new QTreeWidget();
    recent_list->setColumnCount(2);
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
    v_btn_layout->addWidget(new_btn);                           // Second row second col first row
    v_btn_layout->addWidget(browse_btn);                          // Second row second col second row
    v_btn_layout->addWidget(open_btn);                          // Second row second col third row

    for (auto project : RecentProject().load_recent()) {
        QTreeWidgetItem* item = new QTreeWidgetItem(recent_list);
        item->setText(0, QString::fromStdString(std::get<0>(project)));     // Set name
        item->setText(1, QString::fromStdString(std::get<2>(project)));     // Set the last changed date
        item->setToolTip(0, QString::fromStdString(std::get<1>(project)));
        recent_list->addTopLevelItem(item);
    }

    connect(recent_list, &QTreeWidget::itemDoubleClicked, this, &RecentProjectDialog::on_item_double_clicked);
    connect(new_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_new_btn_clicked);
    connect(browse_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_browse_btn_clicked);
    connect(open_btn, &QPushButton::clicked, this, &RecentProjectDialog::on_open_btn_clicked);
}

RecentProjectDialog::~RecentProjectDialog() {
    delete h_layout;
    recent_list->clear();
    delete recent_list;
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
    new_project();
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
    open_project_from_file(project_path);
}

void RecentProjectDialog::on_open_btn_clicked() {
    if (recent_list->selectedItems().length() == 0) return;
    open_project(recent_list->currentItem()->toolTip(0));
    accept();
}
