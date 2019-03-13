#include "videoeditlist.h"

#include "editdialog.h"
#include "generatevideodialog.h"
#include "videoedititem.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QMenu>
#include <QMessageBox>
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
    VideoEditItem* ve_item = new VideoEditItem(start, end, vid_proj, this);
    qDebug() << count();
    addItem(ve_item);
    qDebug() << count();
    repaint();
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
    menu->addAction("Remove", this, &VideoEditList::remove_item);
    menu->addAction("Edit interval", this, [this, pos] { edit_item(itemAt(pos)); });
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
void VideoEditList::remove_item() {
    QMessageBox msgBox;
    msgBox.setText("remove_item");
    msgBox.exec();
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
    int max_width = 0, max_height = 0;

    // Loop over all intervals and save the different sizes in a list
    for (int j = 0; j < count(); ++j) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item(j));
        cv::VideoCapture video_cap;
        video_cap.open(ve_item->get_path().toStdString());
        if (!video_cap.isOpened()) return;
        int width = static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(video_cap.get(CV_CAP_PROP_FRAME_HEIGHT));
        int fps = static_cast<int>(video_cap.get(CV_CAP_PROP_FPS));
        QSize s(width, height);

        if (std::find(sizes.begin(), sizes.end(), s) == sizes.end()) {
            sizes.push_back(s);
        }

        if (std::find(fpses.begin(), fpses.end(), fps) == fpses.end()) {
            fpses.push_back(fps);
        }

        max_width = std::max(width, max_width);
        max_height = std::max(height, max_height);

        video_cap.release();
    }

    if (std::find(sizes.begin(), sizes.end(), QSize(max_width, max_height)) == sizes.end()) {
        sizes.push_back(QSize(max_width, max_height));
    }

    // Maybe add some default sizes???

    // Add the sizes to the dialog and ask the user to choose one to resize to.
    GenerateVideoDialog dialog(sizes, fpses, this);
    int res = dialog.exec();
    QString name;
    QSize size;
    int fps;
    dialog.get_values(&name, &size, &fps);

    if (res == dialog.Rejected) {
        qDebug() << "Rejected";
        return;
    } else {
        qDebug() << "Accepted";
    }

    if (name.isEmpty()) name = "video";

    cv::Mat frame;
    cv::VideoWriter vw;
    int fourCC = CV_FOURCC('M', 'J', 'P', 'G');
    cv::Size cv_size(size.width(), size.height());
    vw.open(name.toStdString() + ".avi", fourCC, fps, cv_size);
    if (!vw.isOpened()) {
        qDebug() << "vw not opened";
        return;
    } else {
        qDebug() << "ok";
    }
    // Loop over all items
    for (int i = 0; i < count(); ++i) {
        VideoEditItem* ve_item = dynamic_cast<VideoEditItem*>(item(i));
        cv::VideoCapture capture(ve_item->get_path().toStdString());
        if (!capture.isOpened()) continue;
        capture.set(CV_CAP_PROP_POS_FRAMES, ve_item->get_start());
        int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

        // Wrong when width > height but height > size height
        bool width_bigger = (width >= height);

        // Should instead of if width > height be something like which one is closer
        if (height == 1004) width_bigger = false;


        int max_dim = (width_bigger) ? width : height;
        float scale = (width_bigger) ? static_cast<float>(size.width()) / max_dim
                                     : static_cast<float>(size.height()) / max_dim;



        // Iterate over the interval
        //while (true) {
        while (capture.get(CV_CAP_PROP_POS_FRAMES) <= ve_item->get_end()) {
            qDebug() << "nr" << capture.get(CV_CAP_PROP_POS_FRAMES);
            if (!capture.read(frame)) {
                break;
            }

            cv::Mat resized = cv::Mat::zeros(cv_size, frame.type());
            qDebug() << "resized size" << resized.cols << resized .rows;

            int f_width = frame.cols, f_height = frame.rows;

            cv::Rect roi;
            if (width_bigger) {
                roi.width = size.width();
                roi.x = 0;
                roi.height = f_height * scale;
                roi.y = (size.height() - roi.height) / 2;
            } else {
                roi.y = 0;
                roi.height = size.height();
                roi.width = f_width * scale;
                roi.x = (size.width() - roi.width) / 2;
            }

            qDebug() << "roi size" << roi.width << roi.height;

            qDebug() << "before resize";
            // Resize

            cv::resize(frame, resized(roi), roi.size());
            qDebug() << "after resize";

            vw.write(resized);
        }
        capture.release();
    }
    qDebug() << "frames" << vw.get(CV_CAP_PROP_FRAME_COUNT);
    vw.release();



    QString str;

    for(int i = 0; i < count(); ++i) {
        QString tmp = item(i)->text();
        str = str + "\n" + tmp;
    }
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();

}
