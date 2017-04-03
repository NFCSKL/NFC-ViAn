#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <string>
#include <QCloseEvent>
#include <QSlider>
#include <QFileDialog>
#include "Library/customdialog.h"
#include "Video/video_player.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
#include "icononbuttonhandler.h"
#include "ui_mainwindow.h"
#include "Filehandler/filehandler.h"
#include "inputwindow.h"
#include "action.h"
#include "qtreeitems.h"
#include <QMutex>
#include <QWaitCondition>

using namespace std;
class inputwindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void set_status_bar(string status, int timer = 750);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void input_switch_case(ACTION action, QString qInput);
    bool eventFilter(QObject *obj, QEvent *event); //cannot follow namestandard, generated code
    const std::string ARROW_STRING = " <--";
    const int SLIDER_UPDATE_TIMER = 200;

    // Lock and wait condition to sleep player when video is paused
    QMutex mutex;
    QWaitCondition paused_wait;

signals:
    void set_play_video();
    void set_pause_video();
    void set_stop_video();
    void resize_video_frame(int width, int height);
    void next_video_frame();
    void prev_video_frame();
    void set_playback_frame(int frame, bool show_frame = false);

private slots:

    void on_playPauseButton_clicked();

    void on_stopButton_clicked();

    void on_actionExit_triggered();

    void set_shortcuts();

    void closeEvent (QCloseEvent *event);

    void update_video(QImage frame);

    void set_video_slider_pos(int pos);

    void resizeEvent(QResizeEvent* event);

    void on_previousFrameButton_clicked();

    void on_nextFrameButton_clicked();

    void on_fastBackwardButton_clicked();

    void on_fastForwardButton_clicked();

    void on_bookmarkButton_clicked();

    void on_videoSlider_sliderPressed();

    void on_videoSlider_sliderReleased();

    void on_videoSlider_valueChanged(int new_pos);

    void on_actionAddProject_triggered();

    void on_ProjectTree_itemClicked(QTreeWidgetItem *item, int column);
    
    void on_actionShow_hide_overlay_triggered();

    void on_actionColour_triggered();

    void on_actionRectangle_triggered();

    void on_actionCircle_triggered();

    void on_actionArrow_triggered();

    void on_actionLine_triggered();

    void prepare_menu(const QPoint & pos);

    void play_video();

    void on_actionSave_triggered();

    void on_actionPen_triggered();

    void on_actionText_triggered();

    void on_actionUndo_triggered();

    void on_actionClear_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionLoad_triggered();

    void on_actionAddVideo_triggered();

    void on_actionChoose_Workspace_triggered();

    void on_actionDeleteProject_triggered();

    void on_actionShow_hide_analysis_area_triggered();

    void on_actionContrast_Brightness_triggered();

private:

    Ui::MainWindow *ui;
    inputwindow *inputWindow;
    video_player* mvideo_player;
    IconOnButtonHandler *iconOnButtonHandler;
    QSlider *video_slider;

    bool slider_blocked = false;
    bool slider_moving = false;
    bool slider_paused_video = false;
    int prev_slider_pos = 0;

    std::chrono::milliseconds slider_timer = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            );

    FileHandler *fileHandler;

    void setup_video_player(video_player *mplayer);
    void set_selected_project(MyQTreeWidgetItem *newSelectedProject);
    void set_selected_video(MyQTreeWidgetItem *newSelectedVideo);
    void set_selected(MyQTreeWidgetItem *&selected, MyQTreeWidgetItem *new_selected);
    void add_project_to_tree(Project* proj);
    void add_video_to_tree(MyQTreeWidgetItem *project, std::string filePath);

    void remove_selected_project_from_tree();
    void remove_video_from_tree(MyQTreeWidgetItem *video);

    void toggle_toolbar();
    void enable_video_buttons();

    MyQTreeWidgetItem *selectedProject;
    MyQTreeWidgetItem *selectedVideo;
    void on_slider_moving();
    void on_slider_click(int new_pos, QPoint local_mouse_pos);
};

#endif // MAINWINDOW_H
