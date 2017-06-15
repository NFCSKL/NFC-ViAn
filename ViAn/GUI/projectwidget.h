#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    void add_project(std::string);

signals:

public slots:
};

#endif // PROJECTWIDGET_H
