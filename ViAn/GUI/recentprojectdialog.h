#ifndef RECENTPROJECTDIALOG_H
#define RECENTPROJECTDIALOG_H

#include <QDialog>

class QHBoxLayout;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class QVBoxLayout;

/**
 * @brief The RecentProjectDialog class
 * Startup dialog used to open recently used projects
 * create new or open an existing one.
 */
class RecentProjectDialog : public QDialog {
    Q_OBJECT
    QHBoxLayout* h_layout;
    QVBoxLayout* v_main_layout;
    QVBoxLayout* right_layout;
    QVBoxLayout* v_btn_layout;
    QVBoxLayout* exit_layout;
    QPushButton* new_btn;
    QPushButton* browse_btn;
    QPushButton* open_btn;
    QPushButton* remove_btn;
    QPushButton* cancel_btn;
    QPushButton* exit_btn;
    QTreeWidget* recent_list;

    const int NUM_COLUMNS = 2;
public:
    RecentProjectDialog(QWidget* parent = nullptr);
    ~RecentProjectDialog();
signals:
    void open_project(QString project_path);
    void open_project_from_file(QString);
    void new_project(void);
    void remove_project();
    void exit();
private slots:
    void item_selection_changed();
    void on_item_double_clicked(QTreeWidgetItem *item);
    void on_new_btn_clicked();
    void on_browse_btn_clicked();
    void on_open_btn_clicked();
    void on_remove_btn_clicked();
    void on_cancel_btn_clicked();
    void on_exit_btn_clicked();
};

#endif // RECENTPROJECTDIALOG_H
