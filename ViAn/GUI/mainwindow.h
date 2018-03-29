#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QDockWidget>
#include <QMainWindow>
#include <QBasicTimer>
#include <QCloseEvent>
#include <QWindow>
#include <QSlider>
#include <QFileDialog>
#include <QDragEnterEvent>

#include <string>

#include "Library/customdialog.h"
#include "reportgenerator.h"

#include "GUI/recentprojectdialog.h"
#include "Project/Analysis/analysisproxy.h"


#include "videowidget.h"
#include "projectwidget.h"
#include "Analysis/analysiswidget.h"
#include "Bookmark/bookmarkwidget.h"
#include "drawingwidget.h"
#include "statusbar.h"
#include "Toolbars/drawingtoolbar.h"

using namespace std;
class AnalysisWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:    
    const int SIZE_MULTIPLIER = 40;
    const int VIDEO_WGT_WIDTH = 16; // 16:9 aspect ratio
    const int VIDEO_WGT_HEIGHT = 9;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    StatusBar* status_bar;
    DrawingToolbar* draw_toolbar;
    QAction* detect_intv_act;
    QAction* bound_box_act;
    QAction* interval_act;
    QAction* ana_details_act;
    QAction* drawing_act;

    QAction* color_act;

    RecentProjectDialog* rp_dialog;

private slots:
    void gen_report(void);
    void cont_bri(void);
    void export_images();
    void rectangle();
    void circle();
    void line();
    void arrow();
    void pen();
    void text();
    void update_text(QString, Shapes*);
    void clear(int frame);
    void clear_current();
    void delete_drawing(Shapes* shape);
    void delete_current_drawing();
    void zoom();
    void move();
    void set_ana_details(bool);

public slots:
    void options(void);
    void open_project_dialog();
    void show_analysis_dock(bool);

signals:
    void set_status_bar(QString);
    void open_project(QString proj_path);

private:
    QDockWidget* queue_dock;

    VideoWidget* video_wgt;
    ProjectWidget* project_wgt;
    AnalysisWidget* analysis_wgt;
    BookmarkWidget* bookmark_wgt;
    DrawingWidget* drawing_wgt;
    QueueWidget* queue_wgt;

    QAction* toggle_project_wgt;
    QAction* toggle_bookmark_wgt;
    QAction* toggle_drawing_wgt;
    QAction* toggle_queue_wgt;

    AnalysisWindow *analysis_window;

    void init_file_menu();
    void init_edit_menu();
    void init_view_menu();
    void init_analysis_menu();
    void init_interval_menu();
    void init_tools_menu();
    void init_help_menu();

    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
