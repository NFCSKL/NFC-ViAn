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
    const QIcon check_icon = QIcon("../ViAn/Icons/check.png");
    const QIcon cross_icon = QIcon("../ViAn/Icons/abort.png");

    std::vector<Video*> m_video_list;
public:
    ViewPathDialog(std::vector<Video*> video_list, QWidget* parent = nullptr);

private:
    void add_paths(QTreeWidget *tree);
    void set_icon(ViewPathItem *item);
    void folder_btn_clicked();
    void browse_btn_clicked();
    void path_btn_clicked();
    void ok_btn_clicked();
    void cancel_btn_clicked();
};

#endif // VIEWPATHDIALOG_H
