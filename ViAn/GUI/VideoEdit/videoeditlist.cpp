#include "videoeditlist.h"

#include "constants.h"
#include "editdialog.h"
#include "generatevideodialog.h"
#include "utility.h"
#include "videoedititem.h"
#include "videogenerator.h"
#include "videointerval.h"
#include "Project/project.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProgressDialog>
#include <QThread>
#include <QDebug>

#include <iostream>

VideoEditList::VideoEditList()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setFlow(QListView::TopToBottom);
    horizontalLayout = false;
    setIconSize(QSize(50,50));

    connect(this, &VideoEditList::customContextMenuRequested, this, &VideoEditList::context_menu);
}

void VideoEditList::add_interval(int start, int end, VideoProject* vid_proj) {
    VideoInterval* vid_interval = new VideoInterval(start, end, vid_proj);
    m_proj->add_interval(vid_interval);

    VideoEditItem* ve_item = new VideoEditItem(vid_interval, this);
    addItem(ve_item);
    repaint();
}

void VideoEditList::set_project(Project* proj) {
    m_proj = proj;
    clear();
    load_intervals();
}

void VideoEditList::load_intervals() {
    for (VideoInterval* interval : m_proj->get_intervals()) {
        VideoEditItem* ve_item = new VideoEditItem(interval, this);
        addItem(ve_item);
    }
    sortItems();
}

void VideoEditList::context_menu(const QPoint &point) {
    if(itemAt(point)) {     // R-clicked on item
        clicked_item = itemAt(point);
        setCurrentItem(clicked_item);
        item_right_clicked(point);
    }
    else{                   // R-clicked not on item
        setCurrentItem(nullptr);
        QMenu* menu = new QMenu;
        menu->addAction("Toggle horizontal/vertical view", this, &VideoEditList::toggle_viewlayout);
        menu->addSeparator();
        menu->addAction("Show video", this, &VideoEditList::show_video);
        menu->addAction("Generate Video", this, &VideoEditList::generate_video);
        menu->exec(mapToGlobal(point));
        delete menu;
    }
}


/**
 * @brief VideoEditList::item_right_clicked
 * Will display the context menu when a videoclip or the list is clicked
 * @param pos
 */
void VideoEditList::item_right_clicked(const QPoint pos) {
    QMenu* menu = new QMenu;
    menu->addAction("Edit interval", this, [this, pos] { edit_item(itemAt(pos)); });
    menu->addAction("Remove", this, [this, pos] { remove_item(itemAt(pos)); });
    menu->addSeparator();
    menu->addAction("Toggle horizontal/vertical view", this, &VideoEditList::toggle_viewlayout);
    menu->addAction("Show video", this, &VideoEditList::show_video);
    menu->addAction("Generate Video", this, &VideoEditList::generate_video);

    menu->exec(mapToGlobal(pos));
    delete menu;
}

void VideoEditList::edit_item(QListWidgetItem* item) {
    VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item);
    int start = ve_item->get_start();
    int end = ve_item->get_end();
    EditDialog dialog(&start, &end, ve_item->get_last_frame());
    if(dialog.exec()) {
        ve_item->set_start(start);
        ve_item->set_end(end);
    }
}

/**
 * @brief VideoEditList::remove_item
 * Removes the clicked list item
 */
void VideoEditList::remove_item(QListWidgetItem* item) {
    VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item);
    VideoInterval* interval = ve_item->get_interval();
    m_proj->remove_interval(interval);
    delete ve_item;
}


/**
 * @brief VideoEditList::toggle_viewlayout
 * Toggles view beween horizontal and vertical
 */
void VideoEditList::toggle_viewlayout() {
    if(horizontalLayout) {
        setFlow(QListView::TopToBottom);
        setIconSize(QSize(50,50));
    } else {
        setFlow(QListView::LeftToRight);
        setIconSize(QSize(110,110));
    }

    horizontalLayout = !horizontalLayout;
}


/**
 * @brief VideoEditList::show_video
 * Shows the video from the videoclip-items in list
 */
// Todo make this do something or remove
void VideoEditList::show_video() {
    QString str;

    for(int i = 0; i < selectedItems().count(); ++i)
    {
        // QListWidgetItem* item = item(i);
        QString tmp = selectedItems().at(i)->text();
        str = str + "\n" + tmp;
    }
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

/**
 * @brief VideoEditList::generate_video
 * Generates video from the videoclip-items in list
 */
void VideoEditList::generate_video() {
    if (count() == 0) return;

    std::vector<QSize> sizes;
    std::vector<int> fpses;
    QSize max_size;
    get_video_info(&sizes, &fpses, &max_size);

    // Add the sizes and frame rates to the dialog
    // and ask the user to choose the wanted variables.
    GenerateVideoDialog dialog(sizes, fpses, this);
    int res = dialog.exec();
    QString name;
    QSize size;
    int fps;
    bool keep_size;
    bool title_screen;
    QString title_text;
    dialog.get_values(&name, &size, &fps, &keep_size, &title_screen, &title_text);

    if (res == dialog.Rejected) return;
    if (name.isEmpty()) name = "video";
    if (keep_size) size = max_size;

    // Create the path from the genereated video
    QString video_folder_path = m_proj->get_dir() + Constants::GENERATED_VIDEO_FOLDER;
    // Create the folder for the generated video
    if (!QDir().mkpath(video_folder_path)) return;

    name = video_folder_path + name;
    name = Utility::add_serial_number(name, ".mkv");

    VideoGenerator* vid_gen = new VideoGenerator(this, name, size, fps, keep_size, title_screen, title_text);

    QProgressDialog* progress = new QProgressDialog(
                "Generating video... 0/" + QString::number(count()), "Abort",
                0, 0, this, Qt::WindowMinimizeButtonHint);
    progress->setAutoClose(false);

    connect(progress, &QProgressDialog::canceled, vid_gen, &VideoGenerator::abort);
    connect(vid_gen, &VideoGenerator::update_progress, progress, &QProgressDialog::setValue);
    connect(vid_gen, &VideoGenerator::update_range, progress, &QProgressDialog::setRange);
    connect(vid_gen, &VideoGenerator::update_text, progress, &QProgressDialog::setLabelText);

    QThread* gen_thread = new QThread;
    vid_gen->moveToThread(gen_thread);
    connect(gen_thread, &QThread::started, vid_gen, &VideoGenerator::generate_video);
    connect(vid_gen, &VideoGenerator::finished, gen_thread, &QThread::quit);
    connect(vid_gen, &VideoGenerator::finished, vid_gen, &VideoGenerator::deleteLater);
    connect(vid_gen, &VideoGenerator::finished, progress, &QProgressDialog::close);
    connect(vid_gen, &VideoGenerator::finished, this, [this, name] { add_video(name); });
    connect(gen_thread, &QThread::finished, gen_thread, &QThread::deleteLater);
    progress->show();
    gen_thread->start();
}

/**
 * @brief VideoEditList::get_video_info
 * @param sizes
 * @param fpses
 * @param max_size
 * Iterate over the items in the list and save all the different
 * resolutions and frame rates to lists
 */
void VideoEditList::get_video_info(std::vector<QSize>* sizes, std::vector<int>* fpses, QSize* max_size) {
    int max_width = 0, max_height = 0;

    // Loop over all intervals and save the different sizes and frame rates in lists
    for (int j = 0; j < count(); ++j) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item(j));
        int rotation = ve_item->get_interval()->get_state().rotation;
        cv::VideoCapture video_cap;
        video_cap.open(ve_item->get_path().toStdString());
        if (!video_cap.isOpened()) return;
        int width = static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        int fps = static_cast<int>(video_cap.get(CV_CAP_PROP_FPS));
        if (rotation == Constants::DEGREE_90 || rotation == Constants::DEGREE_270) {
            std::swap(width, height);
        }

        QSize s(width, height);
        // Store video size
        if (std::find(sizes->begin(), sizes->end(), s) == sizes->end()) {
            sizes->push_back(s);
        }
        // Store frame rate
        if (std::find(fpses->begin(), fpses->end(), fps) == fpses->end()) {
            fpses->push_back(fps);
        }
        // Store the greatest size among the videos
        max_width = std::max(width, max_width);
        max_height = std::max(height, max_height);

        video_cap.release();
    }

    *max_size = QSize(max_width, max_height);

    if (std::find(sizes->begin(), sizes->end(), *max_size) == sizes->end()) {
        sizes->push_back(*max_size);
    }
}

void VideoEditList::save_item_data() {
    for (int i = 0; i < count(); ++i) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item(i));
        ve_item->get_interval()->set_index(i);
    }
}

void VideoEditList::mouseDoubleClickEvent(QMouseEvent* event) {
    if (!itemAt(event->pos())) return;
    VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(itemAt(event->pos()));

    // Start at the first frame of the interval
    VideoState state;
    state = ve_item->get_interval()->get_state();
    state.frame = ve_item->get_start();

    emit set_video(ve_item->get_interval()->get_vid_proj(), state);
}
