#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AnalysisProxy;
class AnalysisWidget;
class AnaSettingWidget;
class BookmarkWidget;
class DrawingToolbar;
class DrawingWidget;
class MainToolbar;
class ManipulatorWidget;
class ProjectWidget;
class QueueWidget;
class RecentProjectDialog;
class Shapes;
class StatusBar;
class YoloWidget;
class VideoProject;
class VideoWidget;
class ZoomPreviewWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:    
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    StatusBar* status_bar;
    DrawingToolbar* draw_toolbar;
    MainToolbar* main_toolbar;
    QAction* detect_intv_act;
    QAction* bound_box_act;
    QAction* interval_act;
    QAction* ana_details_act;
    QAction* drawing_act;

    QAction* color_act;

    RecentProjectDialog* rp_dialog;

private slots:
    void cont_bri(void);
    void export_images();
    void help_clicked();
    void update_text(QString, Shapes*);
    void clear(int frame);
    void delete_drawing(Shapes* shape);
    void delete_current_drawing();
    void zoom();
    void move();
    void open_widget(VideoProject *vid_proj);
    void close_widget(VideoWidget*);
    void close_all_widgets();
    void open_yolo_widget(AnalysisProxy*);

public slots:
    void options(void);
    void open_project_dialog();
    void open_project_folder();
    void view_paths();
    void show_analysis_dock(bool);
    void show_ana_settings_dock(bool);
    void show_bookmark_dock(bool);

signals:
    void set_status_bar(QString);
    void open_project(QString proj_path);

private:
    QDockWidget* bookmark_dock;
    QDockWidget* queue_dock;
    QDockWidget* ana_settings_dock;
    QDockWidget* manipulator_dock;
    QDockWidget* videoedit_dock;

    VideoWidget* video_wgt;
    ProjectWidget* project_wgt;
    AnalysisWidget* analysis_wgt;
    BookmarkWidget* bookmark_wgt;
    DrawingWidget* drawing_wgt;
    ZoomPreviewWidget* zoom_wgt;
    QueueWidget* queue_wgt;
    AnaSettingWidget* ana_settings_wgt;
    ManipulatorWidget* manipulator_wgt;
    YoloWidget* yolo_wgt;

    QAction* toggle_project_wgt;
    QAction* toggle_bookmark_wgt;
    QAction* toggle_drawing_wgt;
    QAction* toggle_queue_wgt;
    QAction* toggle_ana_settings_wgt;
    QAction* toggle_zoom_preview_wgt;
    QAction* toggle_manipulator_wgt;
    QAction* toggle_main_toolbar;
    QAction* toggle_drawing_toolbar;
    QAction* toggle_videoedit_wgt;

    std::vector<VideoWidget*> video_widgets;

    void init_rp_dialog();
    void open_rp_dialog();
    void close_program();

    void init_file_menu();
    void init_edit_menu();
    void init_view_menu();
    void init_analysis_menu();
    void init_interval_menu();
    void init_drawings_menu();
    void init_export_menu();
    void init_help_menu();

    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
