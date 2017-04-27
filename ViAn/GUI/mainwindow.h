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
#include "bookmarkview.h"
#include "action.h"
#include "qtreeitems.h"
#include <QMutex>
#include <QWaitCondition>
#include "makeproject.h"
#include "Analysis/analysiscontroller.h"
#define SCROLL_AREA_MARGIN 25

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
    void add_project_to_tree(Project* proj);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void input_switch_case(ACTION action, QString qInput);
    bool eventFilter(QObject *obj, QEvent *event); //can not follow namestandard, generated code
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
    void abort_analysis();

private slots:

    void on_play_pause_button_clicked();

    void on_stop_button_clicked();

    void on_actionExit_triggered();

    void closeEvent (QCloseEvent *event);//can not follow namestandard, generated code

    void update_video(QImage frame);

    void set_video_slider_pos(int pos);

    void resizeEvent(QResizeEvent* event); //can not follow namestandard, generated code

    void on_previous_frame_button_clicked();

    void on_next_frame_button_clicked();

    void on_decrease_speed_button_clicked();

    void on_increase_speed_button_clicked();

    void on_bookmark_button_clicked();

    void on_video_slider_sliderPressed();

    void on_video_slider_sliderReleased();

    void on_video_slider_valueChanged(int new_pos);

    void on_actionAddProject_triggered();
    
    void on_action_show_hide_overlay_triggered();

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

    void on_project_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    
    void on_action_show_hide_analysis_area_triggered();

    void on_actionContrast_Brightness_triggered();

    void on_action_fill_screen_triggered();

    void on_actionRotate_right_triggered();

    void on_actionRotate_left_triggered();

    void on_document_list_itemClicked(QListWidgetItem *item);

    void on_actionDelete_triggered();

    void on_action_original_size_triggered();

    void on_actionInvert_analysis_area_triggered();

    void on_action_close_project_triggered();

    void on_action_show_hide_analysis_overlay_triggered();

    void save_analysis_to_file(Analysis analysis);

    void show_analysis_progress(int progress);

private:

    Ui::MainWindow *ui;
    video_player* mvideo_player;
    AnalysisController* m_acontroller;
    IconOnButtonHandler *icon_on_button_handler;
    BookmarkView* bookmark_view;
    QSlider *video_slider;

    bool slider_blocked = false;
    bool slider_paused_video = false;
    int prev_slider_pos = 0;

    std::chrono::milliseconds slider_timer = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            );

    FileHandler *fileHandler;
    void setup_filehandler();
    void setup_video_player(video_player *mplayer);
    void setup_analysis(AnalysisController *ac);

    void add_video_to_tree(string file_path, ID id);

    void remove_item_from_tree(MyQTreeWidgetItem *my_item);

    void toggle_toolbar();
    void enable_video_buttons();

    void on_slider_moving();
    void on_slider_click(int new_pos, QPoint local_mouse_pos);
    int slider_pos_under_mouse(QPoint local_mouse_pos);
    QTreeWidgetItem *get_project_from_object(QTreeWidgetItem *item);

    bool original_size;

    void deselect_overlay_tool();

};

#endif // MAINWINDOW_H
