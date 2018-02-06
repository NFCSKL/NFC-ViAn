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
#include "statusbar.h"
#include "Toolbars/maintoolbar.h"
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
    void undo();
    void redo();
    void clear();
    void zoom();
    void move();

public slots:
    void options(void);
    void open_project_dialog();
    void enable_project_tools(bool b);

signals:
    void set_status_bar(QString);
    void open_project(QString proj_path);

private:

    VideoWidget* video_wgt;
    ProjectWidget* project_wgt;
    AnalysisWidget* analysis_wgt;
    BookmarkWidget* bookmark_wgt;

    AnalysisWindow *analysis_window;

    QAction* new_project_act;
    QAction* add_vid_act;
    QAction* open_project_act;
    QAction* save_project_act;
    QAction* close_project_act;
    QAction* remove_project_act;
    QAction* gen_report_act;
    QAction* quit_act;

    QAction* cont_bri_act;
    QAction* cw_act;
    QAction* ccw_act;
    QAction* options_act;

    QAction* toggle_project_wgt;
    QAction* toggle_bookmark_wgt;
    QAction* detect_intv_act;
    QAction* bound_box_act;
    QAction* interval_act;
    QAction* drawing_act;
    QAction* show_analysis_queue;

    QAction* analysis_act;

    QAction* tag_interval_act;
    QAction* rm_tag_interval_act;
    QAction* rm_interval_act;

    QAction* export_act;
    QAction* color_act;
    QAction* rectangle_act;
    QAction* circle_act;
    QAction* line_act;
    QAction* arrow_act;
    QAction* pen_act;
    QAction* text_act;
    QAction* undo_act;
    QAction* redo_act;
    QAction* clear_act;
    QAction* zoom_in_act;
    QAction* zoom_out_act;
    QAction* fit_screen_act;
    QAction* reset_zoom_act;

    QAction* help_act;

    QMenu* file_menu;
    QMenu* edit_menu;
    QMenu* view_menu;
    QMenu* analysis_menu;
    QMenu* interval_menu;
    QMenu* tool_menu;
    QMenu* export_menu;
    QMenu* drawing_tools;
    QMenu* help_menu;

    MainToolbar* main_toolbar;
    DrawingToolbar* draw_toolbar;
    QAction* toggle_draw_toolbar;

    void init_file_menu();
    void init_edit_menu();
    void init_view_menu();
    void init_analysis_menu();
    void init_interval_menu();
    void init_tools_menu();
    void init_help_menu();
};

#endif // MAINWINDOW_H
