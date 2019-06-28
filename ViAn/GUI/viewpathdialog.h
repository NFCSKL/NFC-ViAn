#ifndef VIEWPATHDIALOG_H
#define VIEWPATHDIALOG_H

#include <QDialog>
#include <QIcon>

#include <vector>

class QDialogButtonBox;
class QFormLayout;
class QLineEdit;
class QTreeWidget;
class Video;
class ViewPathItem;
class ViewPathDialog : public QDialog
{
    Q_OBJECT
    QDialogButtonBox* btn_box;

    QTreeWidget* path_list;
    const int NUM_COLUMNS = 3;
    const QIcon check_icon = QIcon(":/Icons/check.png");
    const QIcon cross_icon = QIcon(":/Icons/abort.png");

    std::vector<Video*> m_video_list;
public:
    ViewPathDialog(std::vector<Video*> video_list, QWidget* parent = nullptr);

    bool all_valid();

private:
    void add_paths(QTreeWidget *tree);
    void set_icon(ViewPathItem *item);
    void open_folder_btn_clicked();
    void update_path_btn_clicked();
    void update_folder_btn_clicked();
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // VIEWPATHDIALOG_H
