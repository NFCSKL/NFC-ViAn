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
#include "action.h"

#include "Analysis/AnalysisController.h"
#include "Project/Analysis/analysismeta.h"

#include "videowidget.h"
#include "projectwidget.h"
#include "analysiswidget.h"
#include "bookmarkwidget.h"
#include "statusbar.h"

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
private slots:
    void gen_report(void);
    void cont_bri(void);

public slots:
    void options(void);

signals:
    void set_status_bar(QString);

private:

    VideoWidget* video_wgt;
    ProjectWidget* project_wgt;
    AnalysisWidget* analysis_wgt;
    BookmarkWidget* bookmark_wgt;

    QAction* toggle_project_wgt;
    QAction* toggle_bookmark_wgt;

    AnalysisWindow *analysis_window;

    void init_file_menu();
    void init_edit_menu();
    void init_view_menu();
    void init_analysis_menu();
    void init_tools_menu();
    void init_help_menu();
protected:
};

#endif // MAINWINDOW_H
