#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>
#include <QDir>

class ProjectDialog : public QDialog
{
    Q_OBJECT
    QString* m_name;
    QString* m_path;
    QLineEdit* path_text;
    QLineEdit* name_text;
    QDialogButtonBox* btn_box;
    QString m_default_path;

    const int MIN_WIDTH = 400;

    void enable_ok_btn(const bool& enable);
public:
    explicit ProjectDialog(QString* name, QString* path, QWidget *parent = nullptr, QString default_path = QDir::homePath());

signals:
    void project_path(QString name, QString path);
    void open_project(QString proj_path);

public slots:

private slots:
    void browse_btn_clicked(void);
    void ok_btn_clicked(void);
    void cancel_btn_clicked(void);

    void on_name_text_edited(const QString& new_text);
};

#endif // PROJECTDIALOG_H
