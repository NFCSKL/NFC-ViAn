#include "projectwidget.h"

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {

}

void ProjectWidget::add_project(std::string project_name) {
    QTreeWidgetItem *project = new QTreeWidgetItem();
    project->setText(0, QString::fromStdString(project_name));
    addTopLevelItem(project);
}
